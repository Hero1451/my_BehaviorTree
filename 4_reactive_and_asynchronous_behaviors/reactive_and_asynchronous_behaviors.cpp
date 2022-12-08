#include "behaviortree_cpp/bt_factory.h"
#include "behaviortree_cpp/loggers/bt_cout_logger.h"
#include "behaviortree_cpp/loggers/bt_file_logger.h"
#include <behaviortree_cpp/action_node.h>
#include "dummy_nodes.h"

// using namespace BT;
static const char *xml_text = R"(
 <root BTCPP_format="4">
     <BehaviorTree>
        <Sequence>
            <BatteryOK/>
            <SaySomething   message="mission started..." />
            <MoveBase           goal="1;2;3"/>
            <SaySomething   message="mission completed!" />
        </Sequence>
     </BehaviorTree>
 </root>
 )";

// //2st
// static const char *xml_text = R"(
//  <root>
//      <BehaviorTree>
//         <ReactiveSequence>
//             <BatteryOK/>
//             <Sequence>
//                 <SaySomething   message="mission started..." />
//                 <MoveBase           goal="1;2;3"/>
//                 <SaySomething   message="mission completed!" />
//             </Sequence>
//         </ReactiveSequence>
//      </BehaviorTree>
//  </root>
//  )";

// Custom type
struct Pose2D
{
  double x, y, theta;
};


// Template specialization to converts a string to Position2D.
namespace BT
{
    template <> inline Pose2D convertFromString(StringView str)
    {
        // We expect real numbers separated by semicolons
        auto parts = splitString(str, ';');
        if (parts.size() != 3)
        {
            throw RuntimeError("invalid input)");
        }
        else{
            Pose2D output;
            output.x     = convertFromString<double>(parts[0]);
            output.y     = convertFromString<double>(parts[1]);
            output.theta = convertFromString<double>(parts[2]);
            // printf("come in Pose2D convertFromString!!!\n");
            return output;
        }
    }
} // end namespace BT

namespace chr = std::chrono;

class MoveBaseAction : public BT::StatefulActionNode
{
public:
  // Any TreeNode with ports must have a constructor with this signature
  MoveBaseAction(const std::string &name, const BT::NodeConfig &config)
      : StatefulActionNode(name, config)
  {
  }

  // It is mandatory to define this static method.
  static BT::PortsList providedPorts()
  {
    return {BT::InputPort<Pose2D>("goal")};
  }

  // this function is invoked once at the beginning.
  BT::NodeStatus onStart() override;

  // If onStart() returned RUNNING, we will keep calling
  // this method until it return something different from RUNNING
  BT::NodeStatus onRunning() override;

  // callback to execute if the action was aborted by another node
  void onHalted() override;

private:
  Pose2D _goal;
  chr::system_clock::time_point _completion_time;
};

//-------------------------

BT::NodeStatus MoveBaseAction::onStart()
{
  using namespace BT;
  if (!getInput<Pose2D>("goal", _goal))
  {
    throw BT::RuntimeError("missing required input [goal]");
  }
  printf("[ MoveBase: SEND REQUEST ]. goal: x=%f y=%f theta=%f\n",
         _goal.x, _goal.y, _goal.theta);

  // We use this counter to simulate an action that takes a certain
  // amount of time to be completed (200 ms)
  _completion_time = chr::system_clock::now() + chr::milliseconds(220);

  return BT::NodeStatus::RUNNING;
}

BT::NodeStatus MoveBaseAction::onRunning()
{
  using namespace BT;
  // Pretend that we are checking if the reply has been received
  // you don't want to block inside this function too much time.
  std::this_thread::sleep_for(chr::milliseconds(10));

  // Pretend that, after a certain amount of time,
  // we have completed the operation
  if (chr::system_clock::now() >= _completion_time)
  {
    std::cout << "[ MoveBase: FINISHED ]" << std::endl;
    return BT::NodeStatus::SUCCESS;
  }
  return BT::NodeStatus::RUNNING;
}

void MoveBaseAction::onHalted()
{
  printf("[ MoveBase: ABORTED ]");
}

//第二种方式
#define second_way

int main()
{
  using namespace BT;
  BT::BehaviorTreeFactory factory;
  factory.registerSimpleCondition("BatteryOK", std::bind(DummyNodes::CheckBattery));
  factory.registerNodeType<MoveBaseAction>("MoveBase");
  factory.registerNodeType<DummyNodes::SaySomething>("SaySomething");

  auto tree = factory.createTreeFromText(xml_text);

  StdCoutLogger logger_cout(tree);
  FileLogger logger_file(tree, "bt_trace.fbl");

  // Here, instead of tree.tickWhileRunning(),
  // we prefer our own loop.
#ifndef second_way
  std::cout << "--- ticking\n";
  auto status = tree.tickWhileRunning();
  std::cout << "--- status: " << toStr(status) << "\n\n";
#else
  auto status = NodeStatus::RUNNING;

  while (status == NodeStatus::RUNNING)
  {
    // Sleep to avoid busy loops.
    // do NOT use other sleep functions!
    // Small sleep time is OK, here we use a large one only to
    // have less messages on the console.
    tree.sleep(std::chrono::milliseconds(1000));

    std::cout << "--- ticking\n";
    status = tree.tickOnce();
    std::cout << "--- status: " << toStr(status) << "\n\n";
  }
  #endif
  return 0;
}

// --- ticking
// [ Battery: OK ]
// --- status: RUNNING

// --- ticking
// Robot says: mission started...
// --- status: RUNNING

// --- ticking
// [ MoveBase: SEND REQUEST ]. goal: x=1.0 y=2.0 theta=3.0
// --- status: RUNNING

// --- ticking
// --- status: RUNNING

// --- ticking
// [ MoveBase: FINISHED ]
// Robot says: mission completed!
// --- status: SUCCESS