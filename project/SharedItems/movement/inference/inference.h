#pragma once

#include "movement/sample_point.h"
#include "movement/model/pattern_model.h"
#include "movement/model/model_likelihoods.h"
#include <math.h>

namespace ams
{
	namespace movement
	{
		namespace inference
		{
			using namespace model;

			template<const int num_progress_values>
			class reference_points
			{
			private:
				template<typename T>
				static T min(T a, T b) { return a <= b ? a : b; }

				static float get_error_contrib(const movement_vector& vec, const statistics_value& stats, const int idx)
				{
					const auto e = (vec.values[idx] - stats.means[idx]) * stats.inv_std_devs[idx];
					return e * e;
				}

				static float get_sample_error(const movement_vector& vec, const statistics_value& stats)
				{
					auto res = 0.0f;
//#pragma GCC ivdep
					for (auto i = 0; i < movement_vector::num_movement_dimensions; i++)
						res += get_error_contrib(vec, stats, i);
					return res;
				}
			public:
				reference_points(const movement_vector& vec, const pattern_model<num_progress_values>& model, float max_error) : likelihoods{ 0.0 }
				{
					for (auto i = 0; i < num_progress_values; i++)
						likelihoods[i] = min(get_sample_error(vec, model.values[i]), max_error);
				}
				float likelihoods[num_progress_values];
			};


			template<int num_progress_points>
			class speed_vector
			{
			private:
				float sequence_rate_ = 0;
				float progress_points_per_second_ = 0;
				float current_bias_ = 0;
				float progress_scores_[num_progress_points] = {};
				float exp_progress_scores_[num_progress_points] = {};
			public:
				void reset(const float sequences_per_second, const float default_value = 100.0f)
				{
					sequence_rate_ = sequences_per_second;
					progress_points_per_second_ = num_progress_points * sequences_per_second;
					current_bias_ = 0;
					for (auto i = 0; i < num_progress_points; i++)
						progress_scores_[i] = default_value;
				}

				void apply(const float delta_seconds, const float alpha, const reference_points<num_progress_points>& ref_points)
				{
					const auto new_reference = current_bias_ + delta_seconds * progress_points_per_second_;
					const auto delta = static_cast<int>(new_reference);
					const auto new_bias = new_reference - static_cast<float>(delta);

					float new_progress_scores[num_progress_points];

					for (auto i = 0; i < num_progress_points; i++)
					{
						auto prev_index = static_cast<int>(i + num_progress_points - delta) % num_progress_points;
						new_progress_scores[i] = alpha * progress_scores_[prev_index] + (1.0f - alpha) * ref_points.likelihoods[i];
					}
					for (auto i = 0; i < num_progress_points; i++)
					{
						progress_scores_[i] = new_progress_scores[i];
						exp_progress_scores_[i] = expf(-progress_scores_[i]);
					}
					current_bias_ = new_bias;
				}

				float operator [] (int idx) const
				{
					return exp_progress_scores_[idx];
				}
			};


			template<int num_progress_points, int num_speeds>
			class likelihood_matrix
			{
			private:
				const pattern_model<num_progress_points>* underlying_model;
				speed_vector<num_progress_points> speed_vectors[num_speeds];
				int observation_count = 0;
				unsigned long long int last_observation_us = 0;
			public:
				likelihood_matrix() : underlying_model(nullptr), speed_vectors{} {}

				void init(const pattern_model<num_progress_points>* const model )
				{
					underlying_model = model;
					const speed_statistics& stats = underlying_model->speed_stats;
					const float seq_min = 0.5f * stats.sequences_per_second_min;
					const float seq_max = 1.8f * stats.sequences_per_second_max;
					const float seq_step = (seq_max - seq_min) / (num_speeds - 1);
					for (auto s = 0; s < num_speeds; s++)
					{
						auto sequences_per_second = seq_min + static_cast<float>(s) * seq_step;
						speed_vectors[s].reset(sequences_per_second);
					}
				}

				const speed_vector<num_progress_points>& speeds(int idx) const
				{
					return speed_vectors[idx];
				}
				void apply(const uint64 time_us, const float alpha, const movement_vector& vec, float max_error)
				{
					// handle delta time and observation counts
					const auto delta_us = (observation_count == 0) ? 0 : time_us - last_observation_us;
					last_observation_us = time_us;
					observation_count++;

					// calculate reference points
					reference_points<num_progress_points> reference(vec, *underlying_model, max_error);

					// apply the observation
					auto delta_seconds = static_cast<float>(delta_us) / 1000000.0f;
					
					for (auto s = 0; s < num_speeds; s++)
						speed_vectors[s].apply(delta_seconds, alpha, reference);
				}

				void get_likelihoods(model_likelihoods<num_progress_points, num_speeds>& likelihoods)
				{
					for (int s = 0; s < num_speeds; s++)
					{
						const speed_vector<num_progress_points>& vec = speed_vectors[s];
						for (auto i = 0; i < num_progress_points; i++)
							likelihoods.likelihoods[i][s] = vec[i];
					}
				}

				static void get_progress_likelihood(const model_likelihoods<num_progress_points, num_speeds>& model_likelihoods, float values[num_progress_points])
				{
					for (auto i = 0; i < num_progress_points; i++)
					{
						auto v = 0.0f;
						for (int s = 0; s < num_speeds; s++)
							v += model_likelihoods.likelihoods[i][s];
						values[i] = v;
					}
				}

				static void normalize(float values[num_progress_points], float epsilon = 1.0E-3f)
				{
					auto sum = epsilon;
					for (auto i = 0; i < num_progress_points; i++)
						sum += values[i];
					const auto scale = 1.0f / sum;
					for (auto i = 0; i < num_progress_points; i++)
						values[i] *= scale;
				}


				static int get_max_index(const float values[num_progress_points], float& value)
				{
					auto max_index = 0;
					auto max_value = values[max_index];
					for (auto i = 1; i < num_progress_points; i++)
					{
						const auto v = values[i];
						if (v > max_value)
						{
							max_value = v;
							max_index = i;
						}
					}
					value = max_value;
					return max_index;
				}
			};

		}
	}
}