# Reasonable Planning AI

Create an AI that can think for itself and plan a series of actions using only a data driven Editor. Your players will be astounded by the smart resonponsiveness of the AI to your game environment and marvel at it's ability to adapt and act on the fly!

Reasonable Planning AI is a drop in solution to provide a robust design and data driven AI that thinks for itself and plans it's actions accrodingly. One can create simple AI with no Blueprints nor C++. Reasonable Planning AI acheives this through the Reasonable Planning AI Editor. A robust Unreal Engine editor that lets you predefine a set of actions, a set of goals, and a state. All logic is acheived through the use of various data driven constructs. These components put together are known as Reansonabl Plannign AI Composer.

Reasonable Planning AI is also extensible using either Blueprints or C++. You can opt to extend composer with custom `RpaiComposerActionTasks` or go a pure code route and implement the RpaiRpaiCore Rpai components.

Reasonable Plannign AI is also cross-compatible with Behavior Trees and can execute AITasks. It also comes with an extension to integrate Composer designed Reasonable Planning AI into an existing Behavior Tree through a pre-defined BTTask node within the plugin.

Reasonable Planning AI utilizes the visual logger for easier debugging of your AI. Reasonable Planning AI Editor also has a builtin hueristics testing tool. You can define a given starting state, and ending desired state, and visual the goal the AI will select under those conditions as well as the action plan it will execute.

## Design of Reasonable Planning AI

The core design of the Reasonable Planning AI when using the RpaiRpaiCore components and the RpaiRpaiCore RpaiBrainComponent follows a very simple path of execution. This path of execution is illustrated in the below flow chart.

[!Image describing the flow chart for Reasonable Planning AI execution within the RpaiBrainComponent](./Resources/RpaiDesign.drawio.png)

Text description of the above flowchart. A Start Logic method is invoked. A goal is selected, determined by the current state. If a goal is not determined, then the AI will idle once, else the goal defines it's desired state and it is applied to a planner used to determine an action plan. If a plan is not able to be formulated, then it will attempt a new evaluation of a desired goal. If a plan is found, it will continue to execute the plan until there are no more actions available within the plan or a command to interrupt the execution of the plan is received. After any of those exit conditions, a goal is determined from a current state and the process repeats.

There are two layers to the Reasonable Planning AI that make it a robust AI solution for your project. These layers are known as **RpaiRpaiCore** and **Composer**.

### RpaiRpaiCore

There are five (6) RpaiRpaiCore components implementing Reasonable Planning AI that builds the foundation of the plugin.

- RpaiGoalBase
- RpaiActionBase
- RpaiRpaiPlannerBase
- RpaiRpaiReasonerBase
- RpaiBrainComponent

The main execution engine for Reasonable Planning AI is the `RpaiBrainComponent`. This is a `UBrainComponent` from the `AIModule` used to execute AI logic and interact with the rest of the `AIModule` defined components. The two logic driving classes are `RpaiRpaiReasonerBase` and `RpaiRpaiPlannerBase`. The `RpaiRpaiReasonerBase` class is used to provide implementations for determining goals. Out of the box, there are two implementations provided `RpaiReasoners/RpaiRpaiReasoner_DualUtility` and `RpaiReasoners/RpaiRpaiReasoner_AbsoluteUtility`. Please read the documentation of each class to understand thier capabilities. For goal reasoning there is one provided solution. This solution is `RpaiPlanners/RpaiRpaiPlanner_AStar`. This implementation determines an action plan by using the `RpaiGoalBase::DistanceToCompletion` and `RpaiActionBase::ExecutionWeight` functions for the cost heuristic.

`RpaiGoalBase` and `RpaiActionBase` are the classes most developers will implement when not using Composer classes to build a data drive AI within the editor. `RpaiGoalBase` provides functions for determining value (commonly referred to as utility) for a given desired outcome. It also provides functions for determining the effort to accomplish the given goal from a given current state. `RpaiActionBase` provides functions for heuristics to calculate the effort to do an action given a state. It also provides methods for execution. These execution methods are the primary drivers for having the AI act. These functions are similar to the UE baked in AI `BTTasks`.

### Composer

The Composer layer is built on top of the RpaiRpaiCore layer of Reasonable Planning AI. Composer brings the value of Reasonable Planning AI to Game Designers and others without needing to wire Blueprints or write C++. Because Composer is built on top of RpaiRpaiCore, any Programmers will be able to intergrate into the Composer framework by simply by inheriting from one of the RpaiRpaiCore classes or from the extend Composer defined classes. The Composer defined classes are listed below.

- RpaiComposerGoal
- RpaiComposerAction
- RpaiComposerBrainComponent
- RpaiComposerActionTask
- RpaiComposerStateQuery
- RpaiComposerStateMutator
- RpaiComposerDistance
- RpaiComposerWeight
- RpaiComposerBehavior

The relationship of these classes to each other are defined below. For additonal details please refer to the documentation of the classes either in the Unreal Engine Editor or C++ comments. Ultimately the classes are used to configure data queries to provide cost, weight, applicability, and mutations during the goal selection and action planning processes. `RpaiActionTasks` are predefined actions your AI can do within the game world. `RpaiComposerBrianComponent` is an extension of `RpaiBrainComponent` that adds a factory method to create goals and actions plans from the defined `RpaiComposerBehavior` data asset.

- RpaiComposerGoal
-- RpaiStateQuery
-- RpaiComposerDistance
-- RpaiComposerWeight

- RpaiComposerAction
-- RpaiComposerStateQuery
-- RpaiComposerWeight
-- RpaiComposerStateMutator
-- RpaiComposerActionTask

- RpaiComposerBehavior
-- RpaiComposerAction[]
-- RpaiComposerGoal[]
-- RpaiState: SubclassOf

- RpaiComposerBrainComponent
-- RpaiComposerBehavior

To start using Reasonable Planning AI Composer within the Editor, simply create a new DataAsset within your Content folder and select the type RpaiCmposerBehavior. From there you will be able to define and configure your new AI! See below for a simple tutorial.

## Creating a simple AI

You can give Reasonable Planning AI a quick try by configuring a simple AI by following the steps below. The simple AI will have a goal to move towards a target location and an action that involves having the AI walk to that target location. When a Reasonable Planning AI is designed with a 1:1 Goal to Action design, it is the equivalent of creating a Utility AI. This is what this tutorial will create.

1) Create a new Basic world with Navigation

	a) In your project select New -> Level. In the dialogue select "Basic".
	b) Place a Navigation Volume from the "Place Actors" panel. If you do not see it, open by selecting Window -> Place Actors. Search for "Nav" and drag and drop "NavMeshBoundsVolume" into the Level Viewport.
		i) Set Location to 0,0,0
		ii) Set the Brush X and Y values to 10000.0 and the Z value to 1000.0
		iii) To confirm navigation covers the floor, press `P` to visualize the NavMesh. It should have a green overlay.

2) Create an `RpaiComposerBehavior`

	a) For the `StateType` select `RpaiState_Map`
	b) Add a Goal by clicking the plus sign
		i)
		ii)
		iii)
	c) Add an Action by clicking the plus sign
		i)
		ii)
		iii)