# SimpleAI

AI behaviour simluation prototype

version 1
![version 1](_git_assets/ver1.png)

version 2
![version 2](_git_assets/ver2.png)

version 3
![version 3](_git_assets/ver3.png)

version 3 recording
![version 3 recording](_git_assets/ver3_rec.mp4)


## Readme for the project

### What works (kinda?)

So the Guys are moving around, selecting activities(goals - in this demo i use the term interchangeably, altough sometimes i also mean actions that dont finish right away) and finding ways of satisfying them. The system seems to mostly work smoothly and at least for ~5 guys It's mostly fine. Animations are taken from the objects (or from the actions, depending on what's better for the case). It's an open system that should be straightforward to extend with further functionality:

- Coordination could be done on seperate subsystem which would manage the AIs itself
- Since both objects and NPCs are agents they both can perform and receive actions if needed
- The system is random-locked but ready for adding some attributes that need to be satisfied and finding best goals for that

Time system also works and is configurable via plugin config

### What doesnt work

The system has a few bugs and perfomance issues. Major points are SingleTarget use activities (I think I didn't guard multi-use well enough) and the core plan fiding. The actors are also jumping around the scene because of 2 reasons (relaxed distance requirements and no transition montages (I had few animated I think just didn't have time/energy to actually implement them even though it would be rather straightforward right? add slot and just play dynamic slot montage?))

### SingleTarget

It's just bugged so sometimes because of bad action failures I think either agent doesnt fix its state or object is saved as used (or unused when it was used?). 

#### Plan finding

There are possible huge lag spikes in some cases and I couldn't quite catch the actual culprit. It happends when AI gets a goal and a failing action and keeps on asking is it valid - I tried guarding against it with remembering failed actions and limiting updating goals but it still hapens. I also use plain-old Dijkstra for plan fiding (and do it forward-searching not backtracking over the states) and It's obviously less optimal than A*. I had trouble expressing distance requirements over the states (altough now that I'm writing this README 1,5h before deadline I think that the required state could have location taken from target and have just dynamic function calculating heurestic cost (or maybe some function with cutoff when distance is satisfied?)) and I know Dijkstra by heart so I just used it but I know it's not the optimal algorithm.

### What should be done differently

Multi-use: I actually had multiuse of objects (2 people sitting on couch) with ease but the problem was that I had writting this very inefficently and having to recheck structs of available places with IsValid function was just too much for the planner (I did that at BP layer).

The goals are also spawned per object that can satisfy them (or something like that) which means that If we failed sitting at one chair we might try at another. I did it this way because it gives nice natural way of selecting random goal (if we have room with 9 chairs and 1 tv, watching tv will have 10% chance of being drawn at random) but I'm aware its not the best (especially combined with the failure mechanics which try to find another goal, so if we failed goal sit from one actor, we propably will also fail for another one) and can lead to performance and just AI issues (trying to sit down time after time). In the Map I think it would work best if only the sofa and better chairs had sitting goal.

The states are not very clean (or at least could be better, at least when it comes to distance requirements). I had real trouble expressing distance and to be honest couldn't find anything about it. I found few similiar GOAP showcases but they all had actions like GatherWood, which in itself Moved agent around to find the wood which for me seems bad in more simulative GOAP (I could have action PickupController but then again I could write huge Behaviour tree also...? Of course not literally because we can have PickupAction and subclass it to PickupController where we search for item with tags but that also seems bad, to mingle movement with other actions.). 

I semi-incorporated smart objects but in a dirty and forced way - they should be a primary object in the system. Also the GetState in NPC should just gather state changes from Items? Not manually. 

For the path finding I don't have a lot of experience with unreal's navmesh so I used default tools available - altough I had the problem that navmesh hole by objects was too large for some actions/objects and guys couldnt access them. I just replaced cutting out the mesh with giving huge cost penalty to walk over them. That way they dont get stack(at least rarely) but from time to time they walk over objects. I'm not sure if this should be done differently but I'm guessing so.

Class structure I maybe overused interfaces but that's not that big of a deal like previous points.

I have some stuff I added but never used (Like visual interface which I wanted to use for generic widgets etc, just never got it)

SimpleAISubsystem should also have Config class (which could hold things like MaxDepth).
