# Traffic Simulation Implementation Summary

## Overview of the capabilities of the code.


1. **Modularity**: Logic has been moved from the simulation class to appropriate object classes:
   - Vehicle types and parameters are now encapsulated in the `Voertuig` class
   - Traffic lights handle their own state updates
   - Roads, intersections, and bus stops manage their own data

2. **Features Implementation**:
   I Sadly can't give the original document that relates to the use cases, as it is not mine and property of the university of Antwerp.
   
   - Use Case 1.1: Read in traffic situation from input
   - Use Case 1.2: Support vehicle generator
   - Use Case 1.3: Vehicle types with different parameters
   - Use Case 1.4: Bus stops support
   - Use Case 1.5: Intersections support
   - Use Case 2.1: A simple output (containing speed, amount of roads etc.)
   - Use Case 2.2: Graphical impression (both ASCII art and HTML)
   - Use Case 3.1: Simulating vehicle behaviour
   - Use Case 3.2: Simulation of a traffic light (only red and green)
   - Use Case 3.3: Running the simulation automatically
   - Use Case 3.4: Running the simulation with a vehicle generator
   - Use Case 3.5: Different vehicle types with appropriate behavior
   - Use Case 3.6: Bus stop simulation
   - Use Case 3.7: Intersection simulation
   - Use Case 3.10: Orange traffic light support

3. **Code Structure**:
   - File reader with dedicated methods for each element type
   - Error handling and consistency verification
   - Output options including HTML visualization
   - Interactive main program with multiple options
   - Tests written to check if the code works properly.

## Class Structure

1. **Voertuig (Vehicle)**
   - Encapsulates vehicle types and parameters
   - Handles its own movement and acceleration calculations
   - Supports different types (car, bus, emergency vehicles)

2. **Verkeerslicht (Traffic Light)**
   - Manages its own state (red, orange, green)
   - Supports smart traffic lights with vehicle detection
   - Handles orange light transitions

3. **Bushalte (Bus Stop)**
   - Manages waiting buses and timing
   - Tracks bus arrival and departure

4. **Kruispunt (Intersection)**
   - Connects multiple roads
   - Handles vehicle movement between roads

5. **Simulatie (Simulation)**
   - Coordinates all elements but delegates specific behavior
   - Statistics tracking

6. **Output**
   - Multiple output formats (text, graphical, XML, HTML)
   - Enhanced visualization options

## File Structure
 
- **bushalte.h/cpp**: Bus stop implementation
- **kruispunt.h/cpp**: Intersection implementation
- **voertuig.h/cpp**: Vehicle implementation with types
- **verkeerslicht.h/cpp**: Traffic light with orange state
- **situatie.h/cpp**: Core traffic situation class
- **simulatie.h/cpp**: Simulation logic
- **bestandslezer.h/cpp**: XML file parsing
- **output.h/cpp**: Output generation
- **main.cpp**: Interactive program

## How to Use

1. Compile all source files
2. Run the program with an XML file: `./verkeerssimulatie verkeerssituatie_voorbeeld.xml`
3. Use the interactive menu to:
   - Run simulation steps
   - View text or graphical output
   - Export to XML or HTML
   - Toggle automatic vehicle generation

## XML Format

The XML format has been extended to support:
- Vehicle types
- Bus stops
- Intersections
- Smart traffic lights
- Orange light support
- Vehicle generators with types

## Example

The example XML demonstrates all features including:
- Multiple roads with different lengths
- Various vehicle types
- Traffic lights with options
- Bus stops with waiting times
- Intersections connecting roads
- Vehicle generators
