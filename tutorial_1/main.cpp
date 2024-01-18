#include "behaviortree_cpp/bt_factory.h"

using namespace BT;

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

// Example of custom SyncActionNode (synchronous action)
// without ports.
class ApproachObject : public BT::SyncActionNode {
public:
  ApproachObject(const std::string &name) : BT::SyncActionNode(name, {}) {}

  // You must override the virtual function tick()
  BT::NodeStatus tick() override {
    std::cout << "ApproachObject: " << this->name() << '\n';
    return BT::NodeStatus::SUCCESS;
  }
};

// using namespace BT;

// Simple function that return a NodeStatus
BT::NodeStatus CheckBattery() {
  std::cout << "[ Battery: OK ]\n";
  return BT::NodeStatus::SUCCESS;
}

// We want to wrap into an ActionNode the methods open() and close()
class GripperInterface {
public:
  GripperInterface() = default;

  BT::NodeStatus open() {
    open_ = true;
    std::cout << "GripperInterface::open\n";
    return BT::NodeStatus::SUCCESS;
  }

  BT::NodeStatus close() {
    std::cout << "GripperInterface::close\n";
    open_ = false;
    return BT::NodeStatus::SUCCESS;
  }

private:
  bool open_{true}; // shared information
};

int main(int argc, char *argv[]) {

  // We use the BehaviorTreeFactory to register our custom nodes
  BehaviorTreeFactory factory;

  // The recommended way to create a Node is through inheritance.
  factory.registerNodeType<ApproachObject>("ApproachObject");

  // Registering a SimpleActionNode using a lambda/function pointer.
  factory.registerSimpleCondition("CheckBattery",
                                  [](auto &) { return CheckBattery(); });

  // You can also create SimpleActionNodes using methods of a class
  GripperInterface gripper;
  factory.registerSimpleAction("OpenGripper",
                               [&gripper](auto &) { return gripper.open(); });
  factory.registerSimpleAction("CloseGripper",
                               [&gripper](auto &) { return gripper.close(); });

  // Trees are created at deployment-time (i.e. at run-time, but only
  // once at the beginning).

  // IMPORTANT: when the object "tree" goes out of scope, all the
  // TreeNodes are destroyed
  decltype(factory.createTreeFromText("")) tree;
  
  if (argc == 1) {
    std::cout << "Creating from text\n";
    tree = factory.createTreeFromText(xml_text);
  } else {
    std::cout << "Creating from file: " << argv[1] << "\n";
    tree = factory.createTreeFromFile(argv[1]);
  }
  // To "execute" a Tree you need to "tick" it.
  // The tick is propagated to the children based on the logic of the tree.
  tree.tickWhileRunning();
}
