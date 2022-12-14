#include "dummy_nodes.h"
#include "behaviortree_cpp/bt_factory.h"
#include "behaviortree_cpp/loggers/bt_cout_logger.h"
#include "behaviortree_cpp/loggers/bt_file_logger.h"


using namespace BT;

// clang-format off
// static const char* xml_text = R"(
//  <root >
//      <BehaviorTree>
//         <Sequence>
//             <Script code=" msg:='hello world' " />
//             <Script code=" A:=THE_ANSWER; B:=3.14; color:=RED " />
//             <Precondition if="A>B && color != BLUE" else="FAILURE">
//                 <Sequence>
//                   <SaySomething message="{A}"/>
//                   <SaySomething message="{B}"/>
//                   <SaySomething message="{msg}"/>
//                   <SaySomething message="{color}"/>
//                 </Sequence>
//             </Precondition>
//         </Sequence>
//      </BehaviorTree>
//  </root>
//  )";

// clang-format on

int main()
{
  // Simple tree: a sequence of two asycnhronous actions,
  // but the second will be halted because of the timeout.

  BehaviorTreeFactory factory;
  factory.registerNodeType<DummyNodes::SaySomething>("SaySomething");

  enum Color
  {
    RED = 1,
    BLUE = 2,
    GREEN = 3
  };
  // We can add these enums to the scripting language
  factory.registerScriptingEnums<Color>();

  // Or we can do it manually
  factory.registerScriptingEnum("THE_ANSWER", 10);

  auto tree = factory.createTreeFromText(xml_text);
  factory.createTreeFromFile("./use_xml.xml");
  // StdCoutLogger logger_cout(tree);
  // FileLogger logger_file(tree, "bt_trace.fbl");
  tree.tickWhileRunning();

  return 0;
}

/* Expected output:

Robot says: 42.000000
Robot says: 3.140000
Robot says: hello world
Robot says: 1.000000

*/
