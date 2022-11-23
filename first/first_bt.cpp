#include "behaviortree_cpp/bt_factory.h"
#include "behaviortree_cpp/loggers/bt_cout_logger.h"
#include "behaviortree_cpp/loggers/bt_file_logger.h"
// #include "dummy_nodes.h"



// Simple function that return a NodeStatus
BT::NodeStatus CheckBattery()
{
  std::cout << "[ Battery: OK ]" << std::endl;
  return BT::NodeStatus::SUCCESS;
}

// We want to wrap into an ActionNode the methods open() and close()
class GripperInterface
{
public:
  GripperInterface(): _open(true) {}
    
  BT::NodeStatus open() 
  {
    _open = true;
    std::cout << "GripperInterface::open" << std::endl;
    return BT::NodeStatus::SUCCESS;
  }

  BT::NodeStatus close() 
  {
    std::cout << "GripperInterface::close" << std::endl;
    _open = false;
    return BT::NodeStatus::SUCCESS;
  }

private:
  bool _open; // shared information
};


// Example of custom SyncActionNode (synchronous action)
// without ports.
class ApproachObject : public BT::SyncActionNode
{
public:
  ApproachObject(const std::string& name) :
      BT::SyncActionNode(name, {})
  {}

  // You must override the virtual function tick()
  BT::NodeStatus tick() override
  {
    std::cout << "ApproachObject: " << this->name() << std::endl;
    return BT::NodeStatus::SUCCESS;
  }
};

// clang-format off
static const char* xml_text = R"(

 <root BTCPP_format="4" >
     <BehaviorTree ID="MainTree">
        <Sequence name="root_sequence">
            <CheckBattery   name="check_battery"/>
            <OpenGripper    name="open_gripper"/>
            <ApproachObject name="approach_object"/>
            <CloseGripper   name="close_gripper"/>
        </Sequence>
     </BehaviorTree>
 </root>
 )";

// clang-format on

// int main()
// {
//   using namespace BT;

//   MyLegacyMoveTo move_to;

//   // Here we use a lambda that captures the reference of move_to
//   auto MoveToWrapperWithLambda = [&move_to](TreeNode& parent_node) -> NodeStatus {
//     Point3D goal;
//     // thanks to paren_node, you can access easily the input and output ports.
//     parent_node.getInput("goal", goal);

//     bool res = move_to.go(goal);
//     // convert bool to NodeStatus
//     return res ? NodeStatus::SUCCESS : NodeStatus::FAILURE;
//   };

//   BehaviorTreeFactory factory;

//   // Register the lambda with BehaviorTreeFactory::registerSimpleAction

//   PortsList ports = {BT::InputPort<Point3D>("goal")};
//   factory.registerSimpleAction("MoveTo", MoveToWrapperWithLambda, ports);

//   auto tree = factory.createTreeFromText(xml_text);
//   // FileLogger logger_file(tree, "bt_trace.fbl");
//   StdCoutLogger logger_cout(tree);
  
//   tree.tickWhileRunning();

//   return 0;
// }

// /* Expected output:

// Going to: -1.000000 3.000000 0.500000

// */


// using namespace DummyNodes;


int main()
{
  using namespace BT;
  // using namespace DummyNodes;
    // We use the BehaviorTreeFactory to register our custom nodes
  BehaviorTreeFactory factory;

  // The recommended way to create a Node is through inheritance.
  factory.registerNodeType<ApproachObject>("ApproachObject");

  // Registering a SimpleActionNode using a function pointer.
  // Here we prefer to use a lambda,but you can use std::bind too
  // factory.registerSimpleCondition("CheckBattery", [&](){ return CheckBattery(); });
  factory.registerSimpleCondition("CheckBattery",
                                    [](BT::TreeNode &node) { return CheckBattery(); });

  // You can also create SimpleActionNodes using methods of a class.
  GripperInterface gripper;
  // factory.registerSimpleAction("OpenGripper", [&](){ return gripper.open(); } );
  // factory.registerSimpleAction("CloseGripper", [&](){ return gripper.close(); });

  factory.registerSimpleAction("OpenGripper",
                                 [&gripper](BT::TreeNode &node) { return gripper.open(); });
  factory.registerSimpleAction("CloseGripper",
                                 [&gripper](BT::TreeNode &node) { return gripper.close(); });


  // Trees are created at deployment-time (i.e. at run-time, but only 
  // once at the beginning). 
    
  // IMPORTANT: when the object "tree" goes out of scope, all the 
  // TreeNodes are destroyed
   auto tree = factory.createTreeFromText(xml_text);
   StdCoutLogger logger_cout(tree);
   FileLogger logger_file(tree, "bt_trace.fbl");

  // To "execute" a Tree you need to "tick" it.
  // The tick is propagated to the children based on the logic of the tree.
  // In this case, the entire sequence is executed, because all the children
  // of the Sequence return SUCCESS.
  tree.tickWhileRunning();

  return 0;
}

/* Expected output:
*
  [ Battery: OK ]
  GripperInterface::open
  ApproachObject: approach_object
  GripperInterface::close
*/
