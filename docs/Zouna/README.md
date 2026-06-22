# Technical docs

## Zouna Engine

The engine used by ratatouille is **Zouna**, Asobo Studio's in-house engine, for an in depth look at the history of the engine refer to <link to fmtk wiki zouna page>. Zouna is still in use today, having been evolved greatly over the years by the engine team at Asobo Studio. As of writing this the upcoming game "Resonance: A Plague Tale Legacy" is expected to make use of it.

### General description

Zouna is a node based 3D engine, similar to modern engines like unity and godot, meaning that levels are made up of a tree of nodes that have a position, rotation and scale in the world. These nodes would be nothing without attaching some sort of "actor" to them, in this case that's the job of "objects".

An object represents a physical entity in the world, that can either be seen/collided or in general interacted with. For example, a type of object used across all levels is the "surface" object type. Surface is made up of a group of bezier patches, and is used to model the main large scale level geometry, for example a floor or wall, or the terrain in an outdoors level like "somewhere in france". Another example of an object is "mesh" which represents a more typical 3d model made up of triangles, used for props and more detailed geometry in the different levels. This description wont go into further detail on the other object types since it's not the main focus, but you can refer to the <Object types page> for a detailed explanation of each object type.

It is worth noting that it does not use BSP, however it uses its own grid based approach to space partitioning, in order to optimize the performance of collision and visibility testing.

### Code separation

The code is divided in 3 main libraries.

Engine -> Core engine code, it is game and platform agnostic and provides the basic features any game would need, rendering, input management, commands, scripting, animations, to name a few.

LibGC -> GameCube backend code for many of the core engine systems, as well as implementing any extra systems specific to the GameCube. Each platform had a different library (LibPS2, LibPC, LibMAC).

Rat -> Game specific code for Ratatouille, it contains the code related to gameplay logic such as missions, objectives, player and NPC states, among others.

### Architecture

Zouna was developed using the C++ language, and makes heavy use of features that distinguish the language from C. The engine is programmed with an OOP paradigm, making heavy use of inheritance and polymorphism, by the means of virtual methods. Templates are used as well, mainly for the different container data structures the engine provides.

An example of how inheritance is used can be illustrated with the core engine systems, take Renderer_Z for example. This class defines the core renderer features any platform will use, by defining some basic platform agnostic functions and members. Ultimately however, to draw graphics on screen the code needs to interface with the underlying platform's graphics API, for this Renderer_Z defines virtual functions that will need to be overriden. To implement the graphics backend for the given platform (in this case GameCube), a new class is defined, GCRenderer_Z, which inherits from Renderer_Z and defines all the platform specific code, including overrides of virtual functions and new members. Like renderer many of the engine systems that depend on platform specific code are implemented in this way. It's important to take into account that platform specific implementations are not always handled this way, sometimes a class can be used in engine code and linked in from the platform specific library, like FileHdl_Z for example.

Inheritance is key for one of the most powerful systems the engine has, which is its factory (see factory pattern). The way most memory management is done in the engine is through the factory. To properly explain the factory there are 2 relevant classes: HandleManager_Z, and ClassManager_Z (which inherits from HandleManager_Z).

HandleManager_Z is in charge of keeping a list of metadata (HandleRec_Z) for all allocated instances of classes, and it provides a command for the engine to run a garbage collection pass on this list, in which all used instances are marked as used by other objects that depend on them, if an instance is not marked it can safely be deleted, therefore removing the need for a whole lot of manual memory management. The garbage collection is not executed every frame, on the contrary, it's only ran when the game is affected in a way that can mean stale instances were left over, for example after switching to a different level, or finishing a mission, among others. To deal with this automatic memory management, the usage of raw pointers is avoided as much as possible, so that if an instance gets deleted the pointer does not become a stale reference. This is where the namesake of the manager enters the picutre "handles". A handle is basically a smart pointer, that stores the index associated to that instance in the metadata list that handlemanager owns, and an incrementing key that identifies that generation of the handle (in case the metadata slot gets freed and reallocated). HandleManager_Z provides the GetPtr method that looks up the pointer in the list with the index from the handle. This way if a class needs to reference a dependency, for example the world class referencing its root node, instead of a raw pointer a handle is used, and if the handle is valid GetPtr can be called to make use of the root node. One important detail about handles is that they have an operator-> overload, that lets us use them as if they were pointers directly, without explicitly calling GetPtr, the downside to this (mostly for the original programmers, cause we look at the assembly and have a clear objective of what should be generated) is that there can be hidden redundant calls to GetPtr that serve no purpose and could be avoided by caching the pointer in a local with one explicit GetPtr call.

ClassManager_Z is the other half of the picture, which answers the question of how a new object is created in a way that we obtain a valid handle to it. In the engine initialization phase theres a process where all valid classes that can be instantiated from the factory are defined, for this a class must inherit from BaseObject_Z (which most engine classes do), and implement the static method "NewObject", which basically just does `return new <class>`, then when registering the class ClassManager_Z::RegisterClass is called passing 2 strings, the class name and parent class name, and a pointer to the NewObject function for that class. Then when in the code we wanna create a new instance of an object we call one of the overloads of ClassManager_Z::NewObejct and most importantly pass the string for the given class. So for example if we wanted a 3d model we would pass "Mesh_Z". Something remarkable about the factory is that it allows a way to abstract the creation of platform or game specific classes, how? By registering the same class string with a different NewObject method. Take mesh as an example, to draw a 3d model some platform specific data and code are needed, so MeshGC_Z inherits from Mesh_Z, but when registering the class instead of registering a class with the string "MeshGC_Z", we register "Mesh_Z" with MeshGC_Z::NewObject, therefore when anywhere in the code a new mesh is created it will use the proper one for the current platform, and we don't have to worry about handling that manually, this is done for every platform specific class that can be created through the factory pretty much. It goes even further, if for some reason the game code wanted to register its own mesh (which does not make much sense in this case, but to illustrate) it could do so by registering "Mesh_Z" with its own RatMesh_Z::NewObject.

## Object types

### SURFACE_Z

Cubic bezier surface made up of many cubic bezier patches, it's defined by control points and it gets tesellated according to a lod level set by the engine for display and a separate lod level for collision.

### SPLINE_Z

Cubic bezier spline, used to define smooth curves/paths, for example for scripted camera movements or NPC AI movement.

### SKIN_Z

Skinned mesh, it's made up of a list of meshes, together with a bone hierarchy for the application of skeletal animations.

### ROTSHAPE_Z

It's a 2d quad with a texture that can be made to always face the camera vertically/horizontally, used a lot for effects like lights or tree leaves, among many others.

### LOD_Z

It's the "entity" class for the engine, not only does it define an lod chain of other objects (skin/mesh), it also allows to set a behavior for the entity (using the agent scripting system).

### MESH_Z

Static 3d geometry made up of triangles

### CAMERA_Z

A camera that can be used to draw the scene to the viewport.

### SPLINEZONE_Z

List of 2d top down bounds for the world (XZ), on its own it does not have much use but its derived by the next 2 object types to make use of the world zones it defines.

### OCCLUDER_Z

Uses the world zones defined by its splinezone to cull objects the player cannot currently see (depending on the zone the player is in and which other zones are marked as visible from the current one).

### CAMERAZONE_Z

Uses the world zones defined by its splinezone to define different camera modes/parameters depending on the current zone the player is in.

### LIGHT_Z

Represents a directional light in the world (has no position, only rotation), for example as a base sun light for the level. (applies to nodes below the one that owns the object)

### HFOG_Z

Defines linear horizontal fog in the level (applies to nodes below the one that owns the object)

### COLLISIONVOL_Z

It represents a "trigger" in the world that can execute different behaviors depending on its agent when the player touches it/stays in it/leaves it.

### EMITER_Z

Leftover unused class, presumably the original version of what turned into PARTICLES_Z.

### OMNI_Z

Omnidirectional (point) light, it has a position in the world, color, start and end for attenuation and can also be turned into a spotlight.

### GRAPH_Z

It represents a graph of nodes connected to eachother, in rat it's used for the smell-o vision drawing, to show the player the shortest path to their next objective.

### PARTICLES_Z

A list of particle emmitters, particles are 2d quads with a texture and associated parameters to define the way they move and evolve, and finally despawn.

### FLARE_Z

Effect of a camera lens flare when looking at it. Used to define a list of flares with materials.

### HFIELD_Z

Unused

### TREE_Z

Unused

## Managers

Managers to document (sorted by importance):

- Console_Z: The command interpreter system
- ClassManager_Z: The object factory system
- HandleManager_Z: The object lifetime system
- Renderer_Z: The rendering system
- ManipulatorManager_Z: The task scheduling system (document all manipulators)
- ABC_ScriptManager: The scripting system (document all agents)
- StreamManager_Z
- SystemDatas_Z
- GameManager_Z
- WorldManager_Z: The world entity system (document World_Z (SEADS), Object_Z, Node_Z)
- InputPlatForm_Z
- InputEngine_Z
- AramManager_Z
- SurfaceCache_Z
- BaseColSurfaceCache_Z
- ColTriangleCache_Z
- Mat4x4Buffer_Z
- Movie_Z
- AnimationManager_Z
- MaterialManager_Z
- SoundManager_Z
- ParticlesManager_Z
- SaveGame_Z
- VolatileMgr_Z
- ObjectBankManager_Z
- NetManager_Z
- EffectManager_Z

## Other stuff that needs documenting (not in any particular order)

- Debug features
- Bigfiles
- Resource types (material, materialanim, bitmap, userdefine, etc etc)
- Containers
- Custom allocator
- Name_Z (hashes)
- Math classes
- SystemObject classes
- Collision/Movement (ObjectsGame_Z, ObjectMove_Z)
- Clone classes
- PutParameter
- TrText