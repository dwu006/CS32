#include "tp.h"
#include <queue>
#include <vector>
#include <limits>
#include <unordered_map>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

// Define a structure to represent a state in our search
struct State
{
    std::string airport;             // current airport code
    int time;                        // current time (arrival time of last flight or start time)
    int cost;                        // total travel time in seconds (current time - start_time)
    std::vector<FlightSegment> path; // sequence of flights taken
};

// Comparator for the priority queue (min-heap based on cost)
struct StateComparator
{
    bool operator()(const State &a, const State &b) const
    {
        return a.cost > b.cost;
    }
};

// Helper function to format Unix timestamp to UTC time string
std::string format_time_utc(int unix_timestamp) {
    std::time_t time = static_cast<std::time_t>(unix_timestamp);
    std::tm* tm_info = std::gmtime(&time);
    
    std::ostringstream oss;
    oss << std::put_time(tm_info, "%Y-%m-%d %H:%M UTC");
    return oss.str();
}

TravelPlanner::TravelPlanner(const FlightManagerBase& flight_manager, const AirportDB& airport_db) 
    : TravelPlannerBase(flight_manager, airport_db) {}

bool TravelPlanner::plan_travel(std::string source_airport, std::string destination_airport, int start_time, Itinerary& itinerary) const
{
    // Priority queue for states, ordered by total travel time (cost).
    std::priority_queue<State, std::vector<State>, StateComparator> pq;

    // Map to record best arrival time seen for each airport
    std::unordered_map<std::string, int> bestArrival;

    // Initial state: at source_airport, starting at start_time, cost 0, no flights taken yet
    State init;
    init.airport = source_airport;
    init.time = start_time;
    init.cost = 0;
    pq.push(init);
    bestArrival[source_airport] = start_time;

    // Get constraints from travel planner
    int max_duration_sec = this->get_max_duration();
    int min_connection_time = this->get_min_connection_time();
    int max_layover = this->get_max_layover();

    while (!pq.empty())
    {
        State current = pq.top();
        pq.pop();

        // Skip this state if we've found a better way to get to this airport
        if (current.time > bestArrival[current.airport])
            continue;

        // If we have reached the destination_airport, populate the output itinerary and return true
        if (current.airport == destination_airport)
        {
            itinerary.source_airport = source_airport;
            itinerary.destination_airport = destination_airport;
            itinerary.flights = current.path;
            
            // Calculate total duration properly in seconds
            int total_duration_sec = 0;
            
            // Add wait time at initial airport
            if (!current.path.empty()) {
                total_duration_sec += (current.path[0].departure_time - start_time);
            }
            
            // Add flight durations and layovers
            for (size_t i = 0; i < current.path.size(); i++) {
                // Add flight duration
                total_duration_sec += current.path[i].duration_sec;
                
                // Add layover time if not the last flight
                if (i < current.path.size() - 1) {
                    total_duration_sec += (current.path[i+1].departure_time - 
                                     (current.path[i].departure_time + current.path[i].duration_sec));
                }
            }
            
            itinerary.total_duration = total_duration_sec;
            return true;
        }

        // Current travel time so far
        int current_duration = current.time - start_time;
        
        // If we've already exceeded max duration, skip this state
        if (current_duration > max_duration_sec) {
            continue;
        }

        // Calculate earliest and latest departure times based on constraints
        int earliestDeparture = current.time;
        if (!current.path.empty()) {
            earliestDeparture += min_connection_time;
        }
        
        int latestDeparture = current.time + max_layover;

        // Get available flights from current airport
        std::vector<FlightSegment> flights = get_flight_manager().find_flights(current.airport, earliestDeparture, latestDeparture);

        // Process each potential next flight
        for (const auto& flight : flights)
        {
            if (flight.departure_time < earliestDeparture) continue;
            
            // Calculate estimated total duration if we take this flight
            int arrival_time = flight.departure_time + flight.duration_sec;
            int estimated_total_duration = arrival_time - start_time;
            
            // Skip if this would exceed our max duration constraint
            if (estimated_total_duration > max_duration_sec) {
                continue;
            }

            // Create next state
            State next;
            next.airport = flight.destination_airport;
            next.time = arrival_time;
            next.cost = estimated_total_duration;
            next.path = current.path;
            next.path.push_back(flight);

            // Check if this is a better way to reach this airport
            if (bestArrival.find(next.airport) == bestArrival.end() || 
                next.time < bestArrival[next.airport])
            {
                bestArrival[next.airport] = next.time;
                pq.push(next);
            }
        }
    }

    // No valid itinerary found
    return false;
}

void TravelPlanner::add_preferred_airline(std::string airline)
{
    m_preferred_airlines.push_back(airline);
}