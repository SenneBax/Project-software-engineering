/**
 * @file test_simulatie.cpp
 * @brief Tests for the simulatie class
 */

#include <gtest/gtest.h>
#include "test_helpers.h"
#include "../Simulation/simulatie.h"
#include "../Situation/situatie.h"
#include "../TraficObjects/baan.h"
#include "../TraficObjects/voertuig.h"
#include "../TraficObjects/verkeerslicht.h"

// Test for creating a simulation object
TEST(SimulatieTest, ConstructorTest) {
    VerkeersSituatie situatie;
    simulatie sim(situatie, 1.0);
    EXPECT_EQ(sim.getTijdstap(), 1.0);
    EXPECT_EQ(sim.getHuidigeSimulatieTijd(), 0.0);
    EXPECT_EQ(sim.getTotaleTijd(), 0.0);
}

// Test for handling invalid time step
TEST(SimulatieTest, InvalidTimeStepTest) {
    VerkeersSituatie situatie;
    Baan baan("Teststraat", 250);
    situatie.voegBaanToe(baan);

    // Test with zero time step, should use default value 0.0166
    simulatie simZero(situatie, 0.0);
    EXPECT_DOUBLE_EQ(simZero.getTijdstap(), 0.0166);

    // Test with negative time step, should use default value 0.0166
    simulatie simNegative(situatie, -1.0);
    EXPECT_DOUBLE_EQ(simNegative.getTijdstap(), 0.0166);
}

// Test for updating the simulation
TEST(SimulatieTest, StepTest) {
    VerkeersSituatie situatie;
    Baan baan("Teststraat", 250);
    situatie.voegBaanToe(baan);

    auto voertuig = Voertuig::maakVoertuig("Teststraat", 0, "auto");
    situatie.voegVoertuigToe(std::move(voertuig));

    simulatie sim(situatie, 1.0);

    // Initial state
    EXPECT_EQ(0.0, sim.getHuidigeSimulatieTijd());
    EXPECT_EQ(1, sim.getAantalVoertuigen());
    EXPECT_EQ(0.0, sim.getGemiddeldeSnelheid());

    // Take a step
    sim.stap();

    // Time should have advanced
    EXPECT_GT(sim.getHuidigeSimulatieTijd(), 0.0);
    // Total time should match current time at this point
    EXPECT_EQ(sim.getHuidigeSimulatieTijd(), sim.getTotaleTijd());

    // Vehicle should have moved
    EXPECT_GT(situatie.getVoertuigen()[0]->getPositie(), 0.0);
}

// Test for automatically generating vehicles
TEST(SimulatieTest, AutoGenerateVehiclesTest) {
    VerkeersSituatie situatie;
    Baan baan("Teststraat", 250);
    situatie.voegBaanToe(baan);

    // Add a vehicle generator
    VoertuigGenerator generator("Teststraat", 5); // Generate a vehicle every 5 seconds
    situatie.voegVoertuigGeneratorToe(generator);

    simulatie sim(situatie, 1.0);
    sim.setAutoGenereerVoertuigen(true);

    // Initially no vehicles
    EXPECT_EQ(0, sim.getAantalVoertuigen());

    // Run the simulation for 6 seconds (enough to generate at least one vehicle)
    for (int i = 0; i < 6; i++) {
        sim.stap();
    }

    // Should have at least one vehicle now
    EXPECT_GT(situatie.getVoertuigen().size(), 0);

    // Disable auto-generation
    sim.setAutoGenereerVoertuigen(false);
    int vehicleCount = situatie.getVoertuigen().size();

    // Run for 10 more seconds with auto-generation disabled
    for (int i = 0; i < 10; i++) {
        sim.stap();
    }

    // Number of vehicles should not have increased
    EXPECT_EQ(vehicleCount, situatie.getVoertuigen().size());
}

// Test for calculating traffic light statuses
TEST(SimulatieTest, TrafficLightStatusTest) {
    VerkeersSituatie situatie;
    Baan baan("Teststraat", 250);
    situatie.voegBaanToe(baan);

    // Add a traffic light with a 10-second cycle
    Verkeerslicht verkeerslicht("Teststraat", 150, 10, true, false);
    situatie.voegVerkeerslichtToe(verkeerslicht);

    simulatie sim(situatie, 1.0);

    // Place a vehicle just before the traffic light
    auto voertuig = Voertuig::maakVoertuig("Teststraat", 145, "auto");
    voertuig->setSnelheid(5.0); // Start with speed 5 m/s
    situatie.voegVoertuigToe(std::move(voertuig));

    // Traffic lights start red (at time 0), so the vehicle should slow down
    sim.stap();

    // Check if the vehicle is indeed slowing down (indirect test for red light)
    EXPECT_LT(situatie.getVoertuigen()[0]->getVersnelling(), 0);

    // Run simulation for 5 seconds (light should turn green after 5 seconds)
    for (int i = 0; i < 4; i++) {
        sim.stap();
    }

    // After 5 seconds, light should be green, and the vehicle should accelerate again
    sim.stap();

    // Test if the simulation time is correct
    EXPECT_NEAR(sim.getHuidigeSimulatieTijd(), 6.0, 0.001);

    // Vehicle should be accelerating now
    EXPECT_GT(situatie.getVoertuigen()[0]->getVersnelling(), 0);
}

// Test for removing vehicles that reach the end of the road
TEST(SimulatieTest, RemoveVehiclesTest) {
    VerkeersSituatie situatie;
    Baan baan("Teststraat", 100);
    situatie.voegBaanToe(baan);

    // Add a vehicle that is almost at the end of the road
    auto voertuig = Voertuig::maakVoertuig("Teststraat", 95, "auto");
    voertuig->setSnelheid(10.0); // Position 95, speed 10 m/s
    situatie.voegVoertuigToe(std::move(voertuig));

    // Check that we start with one vehicle
    EXPECT_EQ(1, count(situatie.getVoertuigen()));

    simulatie sim(situatie, 1.0);

    // After one step, the vehicle should have left the road and been removed
    sim.stap();

    // Check if the vehicle has been removed
    EXPECT_EQ(0, count(situatie.getVoertuigen()));

    // Check if the counter for removed vehicles has been incremented
    EXPECT_EQ(sim.getTotaalVerwijderdeVoertuigen(), 1);
}

// Test for vehicle following behavior
TEST(SimulatieTest, VehicleFollowingTest) {
    VerkeersSituatie situatie;
    Baan baan("Teststraat", 300);
    situatie.voegBaanToe(baan);

    // Add two vehicles: one slower in front, one faster behind
    auto voertuigVoor = Voertuig::maakVoertuig("Teststraat", 100, "auto");
    voertuigVoor->setSnelheid(5.0); // Slow-moving vehicle (5 m/s)
    situatie.voegVoertuigToe(std::move(voertuigVoor));

    auto voertuigAchter = Voertuig::maakVoertuig("Teststraat", 50, "auto");
    voertuigAchter->setSnelheid(15.0); // Faster vehicle (15 m/s)
    situatie.voegVoertuigToe(std::move(voertuigAchter));

    // Make sure both vehicles initially have positive acceleration
    situatie.getVoertuigen()[0]->setVersnelling(0.5);
    situatie.getVoertuigen()[1]->setVersnelling(1.0);

    simulatie sim(situatie, 0.5); // Half-second per step

    // Run simulation for multiple steps to give vehicles time to adjust
    for (int i = 0; i < 20; i++) {
        sim.stap();

        // After a few steps, the rear vehicle should start slowing down
        if (i >= 5) {
            // Calculate distance between vehicles
            double distance = situatie.getVoertuigen()[0]->getPositie() -
                            situatie.getVoertuigen()[1]->getPositie() -
                            situatie.getVoertuigen()[0]->getLengte();

            // Distance should not be negative (no collision)
            EXPECT_GE(distance, 0);

            // If vehicles are close, the rear one should be slowing down
            if (distance < situatie.getVoertuigen()[1]->getMinVolgafstand() * 1.5) {
                // Rear vehicle's acceleration should be less than
                // the front vehicle's, indicating it's slowing down
                EXPECT_LE(situatie.getVoertuigen()[1]->getVersnelling(),
                        situatie.getVoertuigen()[0]->getVersnelling() + 0.1);
            }
        }
    }

    // After multiple steps, the rear vehicle should be following at a safe distance
    double finalDistance = situatie.getVoertuigen()[0]->getPositie() -
                        situatie.getVoertuigen()[1]->getPositie() -
                        situatie.getVoertuigen()[0]->getLengte();

    // Check that the rear vehicle is not too close to the front one
    EXPECT_GE(finalDistance, 0);

    // Check that the speeds are eventually closer
    double initialSpeedDifference = 10.0; // 15 - 5
    double currentSpeedDifference = std::abs(situatie.getVoertuigen()[1]->getSnelheid() -
                                        situatie.getVoertuigen()[0]->getSnelheid());

    // Speed difference should be less than the original difference
    EXPECT_LT(currentSpeedDifference, initialSpeedDifference);
}

// Test for bus stop behavior
TEST(SimulatieTest, BusStopTest) {
    VerkeersSituatie situatie;

    // Create a test road with a bus stop
    Baan baan("Teststraat", 300);
    situatie.voegBaanToe(baan);

    // Add a bus stop at position 150
    Bushalte bushalte("Teststraat", 150, 10); // 10 seconds wait time
    situatie.voegBushalteToe(bushalte);

    // Add a bus heading toward the bus stop
    auto bus = Voertuig::maakVoertuig("Teststraat", 100, "bus");
    bus->setSnelheid(10.0); // 10 m/s, will reach the bus stop quickly
    situatie.voegVoertuigToe(std::move(bus));

    // Add a car behind the bus (shouldn't stop at the bus stop)
    auto car = Voertuig::maakVoertuig("Teststraat", 50, "auto");
    car->setSnelheid(10.0);
    situatie.voegVoertuigToe(std::move(car));

    simulatie sim(situatie, 0.5); // Half-second per step

    bool busHasStopped = false;
    bool busHasLeftAfterWaiting = false;
    double busPositionWhenStopped = 0.0;

    // Run for up to 50 steps to observe all bus stop behavior
    for (int i = 0; i < 50; i++) {
        sim.stap();

        // Check if the bus has stopped at the bus stop
        auto& voertuigen = situatie.getVoertuigen();
        auto& bushaltes = situatie.getBushaltes();

        if (voertuigen.empty()) {
            break;
        }

        // Find which vehicle is the bus
        Voertuig* busPtr = nullptr;
        Voertuig* carPtr = nullptr;

        for (size_t j = 0; j < voertuigen.size(); j++) {
            if (voertuigen[j]->getType() == "bus") {
                busPtr = voertuigen[j].get();
            } else if (voertuigen[j]->getType() == "auto") {
                carPtr = voertuigen[j].get();
            }
        }

        if (!busPtr) {
            break; // Bus might have reached the end of the road
        }

        // Check if the bus has stopped at the bus stop
        if (!busHasStopped && busPtr->isWaitingAtBusStop() &&
            std::abs(busPtr->getPositie() - 150) < 1.0) {
            busHasStopped = true;
            busPositionWhenStopped = busPtr->getPositie();

            // Also check if the bus stop knows that a bus has stopped
            EXPECT_TRUE(bushaltes[0].isBusGestopt());

            // Bus should be stationary
            EXPECT_NEAR(busPtr->getSnelheid(), 0.0, 0.1);
        }

        // Check if the bus has left after the wait time
        if (busHasStopped && !busPtr->isWaitingAtBusStop() &&
            busPtr->getPositie() > busPositionWhenStopped) {
            busHasLeftAfterWaiting = true;

            // Bus stop should know that the bus has left
            EXPECT_FALSE(bushaltes[0].isBusGestopt());

            // Bus should be moving again
            EXPECT_GT(busPtr->getSnelheid(), 0.0);
            break;
        }

        // Check that the car behaves appropriately
        if (carPtr && carPtr->getPositie() > 150) {
            // Car should just drive past the bus stop
            EXPECT_GT(carPtr->getSnelheid(), 0.0);
        }
    }

    // Check that the bus actually stopped and then left
    EXPECT_TRUE(busHasStopped);
    EXPECT_TRUE(busHasLeftAfterWaiting);

    // Check that the full wait time has elapsed
    EXPECT_GT(sim.getHuidigeSimulatieTijd(), 10.0);
}

// Test for intersection behavior
TEST(SimulatieTest, IntersectionTest) {
    VerkeersSituatie situatie;

    // Create two roads that will intersect
    Baan baan1("Hoofdweg", 300);
    Baan baan2("Zijstraat", 200);
    situatie.voegBaanToe(baan1);
    situatie.voegBaanToe(baan2);

    // Create an intersection between them
    Kruispunt kruispunt;
    kruispunt.voegBaanToe("Hoofdweg", 150);
    kruispunt.voegBaanToe("Zijstraat", 100);
    situatie.voegKruispuntToe(kruispunt);

    // Add a vehicle approaching the intersection
    auto voertuig = Voertuig::maakVoertuig("Hoofdweg", 100, "auto");
    voertuig->setSnelheid(10.0); // Fast enough to reach the intersection quickly
    situatie.voegVoertuigToe(std::move(voertuig));

    simulatie sim(situatie, 0.5); // Half-second per step

    bool vehicleReachedIntersection = false;
    bool vehicleChangedRoad = false;

    // Run simulation for up to 30 steps
    for (int i = 0; i < 30; i++) {
        sim.stap();

        if (situatie.getVoertuigen().empty()) {
            break; // All vehicles have left the simulation
        }

        auto& voertuig = situatie.getVoertuigen()[0];

        // Check if the vehicle has reached the intersection
        if (!vehicleReachedIntersection && voertuig->getPositie() >= 150) {
            vehicleReachedIntersection = true;
        }

        // Check if the vehicle has changed roads
        if (vehicleReachedIntersection && voertuig->getBaanNaam() == "Zijstraat") {
            vehicleChangedRoad = true;
            break;
        }
    }

    // Either the vehicle should have changed roads or left the simulation
    EXPECT_TRUE(vehicleReachedIntersection);
    if (!situatie.getVoertuigen().empty()) {
        EXPECT_TRUE(vehicleChangedRoad);

        // If it changed roads, its position should be after the intersection point
        EXPECT_GT(situatie.getVoertuigen()[0]->getPositie(), 100);
    }
}

// Test for priority vehicles at traffic lights
TEST(SimulatieTest, PriorityVehicleTest) {
    VerkeersSituatie situatie;
    Baan baan("Teststraat", 300);
    situatie.voegBaanToe(baan);

    // Add a traffic light
    Verkeerslicht verkeerslicht("Teststraat", 150, 30); // 30-second cycle
    situatie.voegVerkeerslichtToe(verkeerslicht);

    // Add a regular car
    auto car = Voertuig::maakVoertuig("Teststraat", 100, "auto");
    car->setSnelheid(10.0);
    situatie.voegVoertuigToe(std::move(car));

    // Add a priority vehicle (ambulance)
    auto ambulance = Voertuig::maakVoertuig("Teststraat", 80, "ziekenwagen");
    ambulance->setSnelheid(15.0);
    situatie.voegVoertuigToe(std::move(ambulance));

    simulatie sim(situatie, 0.5); // Half-second per step

    // Run simulation for a few steps
    for (int i = 0; i < 20; i++) {
        sim.stap();
    }

    // Check positions of both vehicles
    if (situatie.getVoertuigen().size() >= 2) {
        // Find the car and the ambulance
        Voertuig* carPtr = nullptr;
        Voertuig* ambulancePtr = nullptr;

        for (size_t i = 0; i < situatie.getVoertuigen().size(); i++) {
            auto& voertuig = situatie.getVoertuigen()[i];
            if (voertuig->getType() == "auto") {
                carPtr = voertuig.get();
            } else if (voertuig->getType() == "ziekenwagen") {
                ambulancePtr = voertuig.get();
            }
        }

        if (carPtr && ambulancePtr) {
            // If the ambulance is behind the car, it might not have reached the light yet
            if (ambulancePtr->getPositie() < carPtr->getPositie()) {
                // Just check that the ambulance is moving faster
                EXPECT_GT(ambulancePtr->getSnelheid(), carPtr->getSnelheid());
            } else {
                // If both have passed the traffic light, the ambulance should be further ahead
                if (carPtr->getPositie() > 150 && ambulancePtr->getPositie() > 150) {
                    EXPECT_GT(ambulancePtr->getPositie(), carPtr->getPositie());
                }
                // If car stopped at the light but ambulance passed it (priority vehicle)
                else if (carPtr->getPositie() < 150 && ambulancePtr->getPositie() > 150) {
                    // This is expected behavior for priority vehicles
                    EXPECT_GT(ambulancePtr->getSnelheid(), 0.0);
                    EXPECT_LT(carPtr->getSnelheid(), ambulancePtr->getSnelheid());
                }
            }
        }
    }
}

// Test for smart traffic lights
TEST(SimulatieTest, SmartTrafficLightTest) {
    VerkeersSituatie situatie;
    Baan baan("Teststraat", 300);
    situatie.voegBaanToe(baan);

    // Add a smart traffic light
    Verkeerslicht verkeerslicht("Teststraat", 150, 30, false, true); // Smart light
    situatie.voegVerkeerslichtToe(verkeerslicht);

    // Add a vehicle approaching the smart light
    auto voertuig = Voertuig::maakVoertuig("Teststraat", 100, "auto");
    voertuig->setSnelheid(5.0); // Slow enough to be detected by the light
    situatie.voegVoertuigToe(std::move(voertuig));

    simulatie sim(situatie, 0.5); // Half-second per step

    // Run for enough steps to see the smart light behavior
    for (int i = 0; i < 30; i++) {
        sim.stap();

        // After running for a while, check if the light has turned green
        if (i >= 20 && situatie.getVerkeerslichten()[0].isGroen()) {
            // Smart light should have detected the vehicle and turned green
            SUCCEED();
            return;
        }
    }

    // If we get here, the test failed because the light didn't turn green
    FAIL() << "Smart traffic light did not turn green for approaching vehicle";
}

// Test the statistics gathering in the simulation
TEST(SimulatieTest, StatisticsTest) {
    VerkeersSituatie situatie;
    Baan baan("Teststraat", 300);
    situatie.voegBaanToe(baan);

    // Add several vehicles with different speeds
    auto car1 = Voertuig::maakVoertuig("Teststraat", 50, "auto");
    car1->setSnelheid(10.0);
    situatie.voegVoertuigToe(std::move(car1));

    auto car2 = Voertuig::maakVoertuig("Teststraat", 100, "auto");
    car2->setSnelheid(15.0);
    situatie.voegVoertuigToe(std::move(car2));

    auto car3 = Voertuig::maakVoertuig("Teststraat", 150, "auto");
    car3->setSnelheid(20.0);
    situatie.voegVoertuigToe(std::move(car3));

    simulatie sim(situatie, 1.0);

    // Run a step to gather statistics
    sim.stap();

    // Check the number of vehicles
    EXPECT_EQ(3, sim.getAantalVoertuigen());

    // Check the average speed (should be (10+15+20)/3 = 15)
    EXPECT_NEAR(15.0, sim.getGemiddeldeSnelheid(), 0.1);

    // Run until all vehicles leave the simulation
    while (sim.getAantalVoertuigen() > 0) {
        sim.stap();
    }

    // All vehicles should have been removed
    EXPECT_EQ(0, sim.getAantalVoertuigen());
    EXPECT_EQ(3, sim.getTotaalVerwijderdeVoertuigen());

    // Average speed should be 0 when there are no vehicles
    EXPECT_NEAR(0.0, sim.getGemiddeldeSnelheid(), 0.001);
}

// Test proper initialization
TEST(SimulatieTest, ProperlyInitialized) {
    VerkeersSituatie situatie;
    simulatie sim(situatie, 1.0);
    EXPECT_TRUE(sim.properlyInitialized());
}