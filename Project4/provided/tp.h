#ifndef TRAVEL_PLANNER_H
#define TRAVEL_PLANNER_H

#include <string>
#include "provided.h" // Assumes definitions for Itinerary, TravelPlannerBase, etc.
#include "fm.h"

class TravelPlanner : public TravelPlannerBase
{
public:
    // Constructor takes FlightManagerBase and AirportDB references as per base class requirement
    TravelPlanner(const FlightManagerBase& flight_manager, const AirportDB& airport_db);

    // Plan a trip from source to destination starting at start_time.
    // Returns an Itinerary with the sequence of FlightSegments and total travel time.
    virtual bool plan_travel(std::string source_airport, std::string destination_airport, int start_time, Itinerary& itinerary) const override;
    virtual void add_preferred_airline(std::string airline) override;

private:
    std::vector<std::string> m_preferred_airlines;
};

#endif 