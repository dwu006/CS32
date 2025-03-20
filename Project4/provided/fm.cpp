#include "fm.h"
#include <fstream>
#include <sstream>
#include <iostream>

FlightManager::FlightManager(const std::string &flightDataFile)
{
    loadFlights(flightDataFile);
}

void FlightManager::loadFlights(const std::string& flightDataFile) {
    std::ifstream infile(flightDataFile);
    if (!infile) {
        std::cerr << "Error opening flight data file: " << flightDataFile << std::endl;
        return;
    }
    std::string line;
    while (std::getline(infile, line)) {
        if(line.empty()) continue;
        std::istringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        if(tokens.size() < 7) continue;
        // Create FlightSegment using the provided constructor
        // FlightSegment(airline, flight_no, source_airport, destination_airport, departure_time, duration_sec)
        FlightSegment fs(tokens[0], std::stoi(tokens[1]), tokens[2], tokens[3], std::stoi(tokens[4]), std::stoi(tokens[6]));
        m_flights.push_back(fs);
    }
}

std::vector<FlightSegment> FlightManager::getFlightsFrom(const std::string &source, int earliestDeparture, int latestDeparture) const
{
    std::vector<FlightSegment> result;
    for (const auto &flight : m_flights)
    {
        if (flight.source_airport == source && flight.departure_time >= earliestDeparture && flight.departure_time <= latestDeparture)
        {
            result.push_back(flight);
        }
    }
    return result;
}

// Implementation of FlightManagerBase's pure virtual methods
bool FlightManager::load_flight_data(std::string filename) {
    loadFlights(filename);
    return !m_flights.empty(); // Return true if flights were loaded
}

std::vector<FlightSegment> FlightManager::find_flights(std::string source_airport, int start_time, int end_time) const {
    return getFlightsFrom(source_airport, start_time, end_time);
}