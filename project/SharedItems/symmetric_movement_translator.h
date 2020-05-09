#pragma once
#include "movement_translator.h"
#include "read_only_collection.h"
#include "pattern_model.h"
#include "inference.h"

namespace ams
{
	namespace movement
	{
		namespace symmetric
		{
			using namespace collections;

			class symmetric_movement_translator final : public movement_translator
			{
			public:
				static const int min_group_count = 10;
				static const int num_progress_values = 100;
				static const int num_speed_points = 20;

				symmetric_movement_translator(const read_only_collection<sample_point>& input_points);
				~symmetric_movement_translator() = default;

				void reset() override;
				bool observe(uint64 time, const movement_vector& movement, movement_activation& activation, float& progress, float& likelihood) override;

				const model::pattern_model<num_progress_values>& model() const { return model_; }
			protected:
				model::pattern_model<num_progress_values> model_;
				model::model_likelihoods<num_progress_values, num_speed_points> likelihoods_;
				inference::likelihood_matrix<num_progress_values, num_speed_points> matrix_;
				int valid_sample_count_;
			};
		}
	}
}
