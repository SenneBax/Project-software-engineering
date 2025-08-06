Traffic Simulation Implementation Summary
Overview of the capabilities of the code.

This project is a modular traffic simulation system written in C++. It models roads, vehicles, traffic lights, intersections, and bus stops, with support for both text and graphical output.

    ⚠️ Note: This was my first programming project, and it is not optimized for performance or scalability. The code is written primarily as a learning experience to understand object-oriented programming and modular system design. While the system is functional and implements many use cases, it could benefit from improvements in efficiency and structure.

1. Modularity

Logic has been moved from the simulation class to the appropriate object classes:

    Vehicle types and parameters are for example encapsulated in the Voertuig class

    Traffic lights handle their own state updates

    Roads, intersections, and bus stops manage their own data and behavior

    etc.

2. Features Implementation

   ⚠️ I cannot provide the original document that relates to the use cases, as it is the property of the University of Antwerp.

   Use Case 1.1: Read in traffic situation from input

   Use Case 1.2: Support vehicle generator

   Use Case 1.3: Vehicle types with different parameters

   Use Case 1.4: Bus stops support

   Use Case 1.5: Intersections support

   Use Case 2.1: A simple output (containing speed, amount of roads etc.)

   Use Case 2.2: Graphical impression (both ASCII art and HTML)

   Use Case 3.1: Simulating vehicle behaviour

   Use Case 3.2: Simulation of a traffic light (only red and green)

   Use Case 3.3: Running the simulation automatically

   Use Case 3.4: Running the simulation with a vehicle generator

   Use Case 3.5: Different vehicle types with appropriate behavior

   Use Case 3.6: Bus stop simulation

   Use Case 3.7: Intersection simulation

   Use Case 3.10: Orange traffic light support

3. Code Structure

   File reader with dedicated methods for each element type

   Error handling and consistency verification

   Output options including HTML visualization

   Interactive main program with multiple options

   Tests written using Google Test to check correctness

Class Structure

    Voertuig (Vehicle)

        Encapsulates vehicle types and parameters

        Handles its own movement and acceleration calculations

        Supports different types (car, bus, emergency vehicles)

    Verkeerslicht (Traffic Light)

        Manages its own state (red, orange, green)

        Supports smart traffic lights with vehicle detection

        Handles orange light transitions

    Bushalte (Bus Stop)

        Manages waiting buses and timing

        Tracks bus arrival and departure

    Kruispunt (Intersection)

        Connects multiple roads

        Handles vehicle movement between roads

    Simulatie (Simulation)

        Coordinates all elements but delegates specific behavior

        Tracks simulation statistics

    Output

        Multiple output formats (text, graphical, XML, HTML)

        Enhanced visualization options

File Structure

    bushalte.h/cpp: Bus stop implementation

    kruispunt.h/cpp: Intersection implementation

    voertuig.h/cpp: Vehicle implementation with types

    verkeerslicht.h/cpp: Traffic light with orange state

    situatie.h/cpp: Core traffic situation class

    simulatie.h/cpp: Simulation logic

    bestandslezer.h/cpp: XML file parsing

    output.h/cpp: Output generation

    main.cpp: Interactive program

    tests/: Contains unit tests using Google Test

How to Use

    Compile all source files in the project:
    
⚠️These must also include the compiled gtest files (release 1.7.0 is the original use).
    These files are available at https://github.com/google/googletest/archive/release-1.7.0.tar.gz and after configuring them you should be left with on linux 2 files named libgtest.a and libgtest_main.a. These files should then be added to the lib file in the gtest/lib directory. And the tests should run when compiled.


g++ *.cpp -o verkeerssimulatie

Run the program with an XML file:

    ./verkeerssimulatie verkeerssituatie_voorbeeld.xml

    Use the interactive menu to:

        Run simulation steps

        View text or graphical output

        Export to XML or HTML

        Toggle automatic vehicle generation

XML Format

The XML format has been extended to support:

    Vehicle types

    Bus stops

    Intersections

    Smart traffic lights

    Orange light support

    Vehicle generators with types

Example

The example XML demonstrates all features including:

    Multiple roads with different lengths

    Various vehicle types

    Traffic lights with options

    Bus stops with waiting times

    Intersections connecting roads

    Vehicle generators
