#ifndef FLIGHT_MANAGER_H
#define FLIGHT_MANAGER_H

#include "provided.h"
#include <string>
#include <vector> // Assumes provided definitions for FlightSegment and FlightManagerBase

class FlightManager : public FlightManagerBase {
public:
    // Constructor that takes a filename
    FlightManager(const std::string& flightDataFile);
    
    // Default constructor that loads from "all_flights.txt"
    FlightManager() : FlightManager("all_flights.txt") {}
    
    void loadFlights(const std::string& flightDataFile);
    
    // Returns all flights departing from a given source between earliest and latest departure times (inclusive).
    std::vector<FlightSegment> getFlightsFrom(const std::string& source, int earliestDeparture, int latestDeparture) const;
    
    // Implementations of FlightManagerBase pure virtual methods
    virtual bool load_flight_data(std::string filename) override;
    virtual std::vector<FlightSegment> find_flights(std::string source_airport, int start_time, int end_time) const override;
    
private:
    std::vector<FlightSegment> m_flights;
};

#endif