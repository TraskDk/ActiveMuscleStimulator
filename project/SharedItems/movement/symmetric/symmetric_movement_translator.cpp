#include "symmetric_movement_translator.h"
#include "movement/analysis/analysis.h"

namespace ams
{
	namespace movement
	{
		movement_translator* create_symmetric_movement_translator(const read_only_collection<sample_point>& input_points)
		{
			return new symmetric::symmetric_movement_translator(input_points);
		}

		namespace symmetric
		{
			using namespace model;
			using namespace analysis;

			const float alpha = 0.96f;
			const float max_error = 100.0f;

			symmetric_movement_translator::symmetric_movement_translator(const read_only_collection<sample_point>& input_points) : matrix_()
			{
				const auto trigger_channel = 0;

				// Group the input sequence into groups, where each group starts when trigger_channel is triggered and ends when re-triggered.
				small_list<small_list<sample_point>> groups;
				make_groups(input_points, trigger_channel, groups);

				// Check that we have a sufficient number of input samples.
				if (groups.length() < min_group_count)
					throw std::runtime_error("Insufficient number of input groups.");

				// Skip first and last group in order to remove noise, but extract a fixed sized pattern from each.
				small_list<pattern_values<num_progress_values>> patterns;
				for (auto i = 1; i < groups.length() - 1; i++)
					make_pattern(groups[i], trigger_channel, patterns.add());

				// Now transform the extracted patterns into a model.
				make_model(patterns, model_);
				reset();
			}

			static const movement_activation no_activation{};

			bool symmetric_movement_translator::observe(uint64 time, const movement_vector& movement, movement_activation& activation, float& progress, float& likelihood)
			{
				float progress_likelihoods[num_progress_values];
				matrix_.apply(time, movement, max_error);
				matrix_.get_likelihoods(likelihoods_);
				matrix_.get_progress_likelihood(likelihoods_, progress_likelihoods);
				matrix_.normalize(progress_likelihoods, 1.0E-7f);

				auto idx = matrix_.get_max_index(progress_likelihoods, likelihood);
				const auto min_likelihood = 0.1f;
				const auto min_valid_sample_count = 25;
				if (likelihood >= min_likelihood)
					valid_sample_count_++;
				else
					valid_sample_count_ = 0;

				activation = no_activation;
				if (valid_sample_count_ < min_valid_sample_count)
				{
					progress = NAN;
					return false;
				}

				if (progress < model_.activation_stats.disable_at_progress)
					activation.channels[0] = true;
				else if (progress >= 0.5 && progress < 0.5 + model_.activation_stats.disable_at_progress)
					activation.channels[1] = true;
							
				activation = no_activation;
				progress = static_cast<float>(idx) * (1.0f / num_progress_values);
				
				return true;
			}

			void symmetric_movement_translator::reset()
			{
				matrix_.init(&model_, alpha);
				valid_sample_count_ = 0;
			}

		}
	}
}