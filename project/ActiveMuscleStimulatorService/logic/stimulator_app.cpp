#include "stimulator_app.h"
#include "movement/sample_point_log.h"
#include "../hardware/raspberry/input_controller.h"
#include "../hardware/raspberry/relay_sbz4.h"
#include "../hardware/raspberry/gyroscope_sensehat_b.h"
#include "../hardware/raspberry/timing_wired.h"
#include <wiringPi.h>
#include <cstdio>
#include <unistd.h>
#include "movement/movement_translator.h"

#include <sys/stat.h>
#include "collections/smart_list.h"

/*
 * Check if a file exist using stat() function
 * return 1 if the file exist otherwise return 0
 */
bool file_exists(const char* filename)
{
	struct stat buffer{};
	const auto exist = stat(filename, &buffer);
	return exist == 0;
}

namespace ams
{
	namespace logic
	{
		static const float gyro_sampling_rate = 100;

		class dummy_translator : public movement::movement_translator
		{
		public:
			void reset() override  {}
			bool observe(const uint64 time, const movement::movement_vector& movement, movement::movement_activation& activation, float& progress, float& likelihood) override { return false; }
		};
		
		stimulator_app::stimulator_app()
		{
			timing_ = new hardware::raspberry::timing_wired();
			input_ = new hardware::raspberry::input_controller();
			relay_ = new hardware::raspberry::relay_sbz4();
			hardware::raspberry::gyroscope_sensehat_b_settings settings;
			settings.sampling_rate = gyro_sampling_rate;
			settings.accel_scale = hardware::raspberry::accel_scale_4G;
			settings.gyro_scale = hardware::raspberry::gyro_scale_1000;
			gyroscope_ = new hardware::raspberry::gyroscope_sensehat_b(settings);
			printf("Gyroscope enabled at frame rate %3.1f.\n", settings.sampling_rate);
		}

		void stimulator_app::reset()
		{
			relay_->set(0, false);
			relay_->set(1, false);
			relay_->set(2, false);
			relay_->set(3, false);
		}


		enum mode {
			tracking_mode,
			test_mode,
			select_mode,
			record_mode
		};

		const char main_file[] = "tracking_file.csv";
		const char recording_file[] = "recording_file.csv";
		const int record_mode_millis = 1000;

	  movement::movement_translator * load_translator(const char * filename)
	  {
			try
			{
				collections::smart_list<movement::sample_point> points;
				if (!movement::sample_point_log::read(filename, points))
				{
					printf("Unable to read data from '%s'.\n", filename);
					return nullptr;
				}
				printf("- loaded %d points from '%s'\n", points.length(), filename);
				const auto translator = create_symmetric_movement_translator(points);
				printf("- created model from '%s'\n", filename);
				return translator;
			}
			catch (const std::exception & ex)
			{
				printf("- unable to model from '%s': %s\n", filename, ex.what());
				return nullptr;
			}
	  }
		
		bool stimulator_app::load_program(int program_index)
		{
			printf("Loading program %d\n", program_index);
			char filename[80];
			sprintf(filename, "saved_file_%d.csv", program_index);
			if (!file_exists(filename)) return false;
			remove(main_file);
			const auto res = symlink(filename, main_file);
			return res >= 0;
		}

		bool stimulator_app::save_program(int program_index)
		{
			printf("Saving program %d\n", program_index);
			if (!file_exists(recording_file)) return false;
			char filename[80];
			sprintf(filename, "saved_file_%d.csv", program_index);
			remove(filename);
			const auto res = rename(recording_file, filename);
			return res >= 0;
		}
		
		void stimulator_app::program()
		{
			auto select_mode_start = 0;
			auto program_select = -1;
			auto mode = tracking_mode;
			auto record_start_time = 0;
			movement::sample_point_log* log = nullptr;
			movement::movement_translator* translator = load_translator(main_file);
			movement::sample_point pt{};

			auto lastmode = -1;

			//auto iteration = 0;
			while (true)
			{
				//iteration++;
				//if ((iteration & 3) == 0)
				//{
				//	printf("\r[%d]", ++iteration);
				//	fflush(stdout);
				//}
				if (lastmode != mode)
				{
					lastmode = mode;
					switch (mode)
					{
					case tracking_mode:
						printf("Now in tracking mode.\n");
						break;
					case record_mode:
						printf("Now in record mode.\n");
						break;
					case test_mode:
						printf("Now in test mode.\n");
						break;
					case select_mode:
						printf("Now in select mode.\n");
						break;
					default:
						printf("Now in mode # %d.\n", mode);
						break;
					}
				}
				
				const auto is_non_blocking_mode = (mode == record_mode) || (mode == tracking_mode && translator != nullptr);
				int button, state;

				if (is_non_blocking_mode)
				{
					while(true)
					{
						if (gyroscope_->read(pt.vec))
							break;
					}
					;
					pt.time_us = micros() - record_start_time;
					if (mode == tracking_mode)
					{
						float progress=0, likelihood=0;
						const auto matched = translator->observe(pt.time_us, pt.vec, pt.act, progress, likelihood);
						if(matched)
						{
							relay_->set(0, pt.act.channels[0] > 0);
							relay_->set(1, pt.act.channels[0] > 0);
							relay_->set(2, pt.act.channels[1] > 0);
							relay_->set(3, pt.act.channels[1] > 0);
						}
						else
						{
							reset();
						}
						//printf("\r%d %4.2f %4.2f %d %d", matched, progress, likelihood, pt.act.channels[0], pt.act.channels[1]);
						//fflush(stdout);
					}
					if (log != nullptr)
					{
						log->append(pt);
					}
				}

				if (input_->get_event(button, state, !is_non_blocking_mode))
				{
					const auto enabled_state = state > 0;
					switch (button)
					{
					case hardware::raspberry::key_r:
						if (mode == tracking_mode && enabled_state == true)
						{
							reset();
							mode = test_mode;
						}
						else if (mode == test_mode && enabled_state == false)
						{
							mode = tracking_mode;
							reset();
							if(translator!=nullptr)
								translator->reset();
						}
						break;
					case hardware::raspberry::key_a:
						if (mode == test_mode || mode == record_mode)
						{
							relay_->set(0, enabled_state);
							relay_->set(1, enabled_state);
							pt.act.channels[0] = enabled_state;
						}
						else if (mode == select_mode && enabled_state == true)
						{
							program_select = 0;
						}
						break;
					case hardware::raspberry::key_b:
						if (mode == test_mode || mode == record_mode)
						{
							relay_->set(2, enabled_state);
							relay_->set(3, enabled_state);
							pt.act.channels[1] = enabled_state;
						}
						else if (mode == select_mode && enabled_state == true)
						{
							program_select = 1;
						}
						break;
					case hardware::raspberry::key_x:
						if (mode == select_mode && enabled_state == true)
						{
							program_select = 2;
						}
						break;
					case hardware::raspberry::key_y:
						if (mode == select_mode && enabled_state == true)
						{
							program_select = 3;
						}
						break;
					case hardware::raspberry::key_start:
						if (enabled_state == true)
						{
							delete log;
							log = nullptr;
							delete translator;
							translator = nullptr;
							return;
						}
						break;
					case hardware::raspberry::key_select:
						if (mode == tracking_mode && enabled_state == true)
						{
							reset();
							mode = select_mode;
							program_select = -1;
							select_mode_start = millis();
						}
						else if(mode == select_mode && enabled_state == false)
						{
							mode = tracking_mode;

							const auto elapsed_ms = millis() - select_mode_start;
							const auto is_record_action = elapsed_ms >= record_mode_millis;

							if (program_select >= 0)
							{
								if (is_record_action)
									save_program(program_select);								
								load_program(program_select);
								delete translator;
								translator = load_translator(main_file);
							}
							else if(is_record_action)
							{
								// start recording
								mode = record_mode;
								//printf("recording mode...\n");
								record_start_time = micros();
								log = movement::sample_point_log::create(recording_file);
							}
						}
						else if(mode == record_mode && enabled_state == false)
						{
							//printf("recording completed...\n");
							delete log;
							log = nullptr;
							mode = tracking_mode;
							delete translator;
							translator = load_translator(recording_file);
						}
					}
				}
			}
		}
		
		void stimulator_app::run()
		{
			while(true)
			{
				reset();
				program();
			}
		}

		stimulator_app::~stimulator_app()
		{
			if(gyroscope_ != nullptr)
			{
				delete gyroscope_;
				gyroscope_ = nullptr;
			}

			if (relay_ != nullptr)
			{
				delete relay_;
				relay_ = nullptr;
			}

			if (input_ != nullptr)
			{
				delete input_;
				input_ = nullptr;
			}
			if (timing_ != nullptr)
			{
				delete timing_;
				timing_ = nullptr;
			}
		}
	}
}
