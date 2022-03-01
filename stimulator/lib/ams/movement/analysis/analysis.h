#pragma once
#include "collections/smart_list.h"
#include "collections/small_list.h"
#include "pattern_values.h"
#include "movement/model/pattern_model.h"
#include <math.h>

namespace ams
{
	namespace movement
	{
		namespace analysis
		{
			using namespace model;
			using namespace collections;

			void make_groups(const read_only_collection<sample_point>& points, int channel, small_list<small_list<sample_point>>& res);

			template<int NumProgressValues>
			void make_pattern(const read_only_collection<sample_point>& sequence, int channel, pattern_values<NumProgressValues>& target)
			{
				const auto& first_sample = sequence.first();
				const auto& last_sample = sequence.last();
				const auto duration = last_sample.time_us - first_sample.time_us;

				auto has_activation_duration = false;
				target.pattern_duration_seconds = static_cast<float>(duration) / 1000000.0f;
				target.activation_duration_seconds = target.pattern_duration_seconds;
				auto idx = 0;
				for (auto i = 0; i < NumProgressValues; i++)
				{
					const auto target_time = first_sample.time_us + duration * i / NumProgressValues;
					while (sequence[idx].time_us < target_time)
						idx++;
					const auto& pt = sequence[idx];
					target.pattern_points[i] = pt;
					if (pt.act.channels[channel] == 0 && !has_activation_duration)
					{
						target.activation_duration_seconds = static_cast<float>(pt.time_us - first_sample.time_us) / 1000000.0f;
						has_activation_duration = true;
					}
				}
			}

			template<int NumProgressValues>
			void get_speed_stats(const read_only_collection<pattern_values<NumProgressValues>>& patterns, speed_statistics& stats)
			{
				float max_value, sum;
				float min_value = max_value = sum = 1.0f / patterns.first().pattern_duration_seconds;
				for (auto i = 1; i < patterns.length(); i++)
				{
					auto sequences_sec = 1.0f / patterns[i].pattern_duration_seconds;
					if (sequences_sec < min_value) min_value = sequences_sec;
					if (sequences_sec > max_value) max_value = sequences_sec;
					sum += sequences_sec;
				}
				stats.sequences_per_second_min = min_value;
				stats.sequences_per_second_max = max_value;
				stats.sequences_per_second_avg = sum / static_cast<float>(patterns.length());
			}

			template<int NumProgressValues>
			void get_activation_stats(const read_only_collection<pattern_values<NumProgressValues>>& patterns, activation_statistics& stats)
			{
				const float activation_sum = 0;
				float deactivation_sum = 0;

				for (auto i = 0; i < patterns.length(); i++)
				{
					const pattern_values<NumProgressValues>& pattern = patterns[i];
					deactivation_sum += pattern.activation_duration_seconds / pattern.pattern_duration_seconds;
				}
				stats.enable_at_progress = activation_sum / static_cast<float>(patterns.length());
				stats.disable_at_progress = deactivation_sum / static_cast<float>(patterns.length());
			}

			template<int NumProgressValues>
			void collect_statistics_value(const read_only_collection<pattern_values<NumProgressValues>>& patterns, int index_point, statistics_value& result)
			{
				float sum[6] = { 0.0f };
				for (auto i = 0; i < patterns.length(); i++)
					for (auto j = 0; j < movement_vector::num_movement_dimensions; j++)
						sum[j] += patterns[i].pattern_points[index_point].vec.values[j];
				float avg[6] = { 0.0f };
				for (int j = 0; j < 6; j++)
				{
					avg[j] = sum[j] / static_cast<float>(patterns.length());
					sum[j] = 0;
				}
				for (auto i = 0; i < patterns.length(); i++)
				{
					for (auto j = 0; j < movement_vector::num_movement_dimensions; j++)
					{
						auto e = patterns[i].pattern_points[index_point].vec.values[j] - avg[j];
						sum[j] += e * e;
					}
				}
				float stddev[6] = { 0.0f };
				for (auto j = 0; j < movement_vector::num_movement_dimensions; j++)
					stddev[j] = sqrtf(sum[j] / static_cast<float>(patterns.length() - 1));

				for (auto j = 0; j < movement_vector::num_movement_dimensions; j++)
				{
					result.means[j] = avg[j];
					result.inv_std_devs[j] = 1.0f / stddev[j];
				}
			}

			template<int NumProgressValues>
			void make_model(const read_only_collection<pattern_values<NumProgressValues>>& patterns, pattern_model<NumProgressValues>& model)
			{
				get_speed_stats(patterns, model.speed_stats);
				get_activation_stats(patterns, model.activation_stats);
				for (auto i = 0; i < NumProgressValues; i++)
					collect_statistics_value(patterns, i, model.values[i]);
			}

		}
	}
}
