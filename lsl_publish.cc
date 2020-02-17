#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math/Vector3.hh>
#include <ignition/math/Pose3.hh>
#include "lsl_cpp.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>


namespace gazebo
{
  class LSLPublish : public ModelPlugin
  {
    public: LSLPublish() : ModelPlugin()
            {
            }

    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/)
    { 
      // Store the pointer to the model
      this->model = _parent;


      //Setup LSL outlet connection
      try {
        //Make a new stream_info (100Hz)
        //info(name, type, channel_count, sample_rate, format, source_id)
        lsl::stream_info info("Gazebo","Position",3,lsl::IRREGULAR_RATE,lsl::cf_float32, std::to_string(this->model->GetId()));
        //Add description fields
        //Add units fields
        info.desc().append_child_value("Units", "Meters");
        //Add Model Name
        info.desc().append_child_value("Model Name", this->model->GetName());
        //Add Model ID
        info.desc().append_child_value("Model ID", std::to_string(this->model->GetId()));


        //Create outlet  
        this->outlet = new lsl::stream_outlet(info);
        
      } 
      catch(std::exception &e) {
		    std::cerr << "Got an exception: " << e.what() << std::endl;
	    }

      // Listen to the update event. This event is broadcast every
      // simulation iteration.
      this->updateConnection = event::Events::ConnectWorldUpdateBegin(
          std::bind(&LSLPublish::OnUpdate, this));
         
    }

    // Called by the world update start event
    public: void OnUpdate()
    {
      // Push Pose of this->model to LSL outlet
       // Get pose of this->model
      ignition::math::Pose3d pose = this->model->WorldPose();
      // Get position vector (XYZ coordinates)
      ignition::math::Vector3<double> v= pose.Pos();

      //Create sample packet and populate with XYZ coordinates
      float sample[3];
      sample[0] = v.X();
      sample[1] = v.Y();
      sample[2] = v.Z();

      //send the sample
      try {
        this->outlet->push_sample(sample);
      }
      catch(std::exception &e) {
		    std::cerr << "Got an exception: " << e.what() << std::endl;
	    }
    }

    // Pointer to the model
    private: physics::ModelPtr model;

    // Pointer to the update event connection
    private: event::ConnectionPtr updateConnection;

    public: lsl::stream_outlet* outlet;
  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(LSLPublish)
}
