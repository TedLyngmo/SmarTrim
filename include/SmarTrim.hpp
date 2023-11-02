#ifndef EVENT_HPP_FAA3D47C_796F_11EE_B771_90B11C0C0FF8
#define EVENT_HPP_FAA3D47C_796F_11EE_B771_90B11C0C0FF8

#include <array>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <execution>
#include <memory>
#include <numeric>
#include <span>
#include <string>
#include <string_view>
#include <vector>

class PerMille {
public:
    using value_type = std::int16_t;

    PerMille() = default;
    PerMille(value_type val) : m_per_mille(std::clamp(val, value_type(0), value_type(1000))) {}
    PerMille(PerMille const&) = default;
    PerMille(PerMille&&) noexcept = default;
    PerMille& operator=(PerMille const&) = default;
    PerMille& operator=(PerMille&&) noexcept = default;
    ~PerMille() = default;

    PerMille& operator+=(PerMille other) noexcept {
        m_per_mille = std::clamp(static_cast<value_type>(m_per_mille + other.m_per_mille), value_type(0), value_type(1000));
        return *this;
    }
    PerMille& operator-=(PerMille other) noexcept {
        m_per_mille = std::clamp(static_cast<value_type>(m_per_mille - other.m_per_mille), value_type(0), value_type(1000));
        return *this;
    }

    value_type value() const noexcept { return m_per_mille; }
    double probability() const noexcept { return static_cast<double>(m_per_mille) / 1000.0; }

private:
    value_type m_per_mille = 0;
};
PerMille operator+(PerMille lhs, PerMille rhs) noexcept {
    return lhs += rhs;
}
PerMille operator-(PerMille lhs, PerMille rhs) noexcept {
    return lhs -= rhs;
}

// A Competitor is a football team, a horse or any kind of participant in an event
class Competitor {
public:
    Competitor(std::string_view name) : m_name(name) {}

private:
    std::string m_name;
};

// A competitor configuration is what's used in a specific event.
class CompetitorConfiguration {
public:
    double probability() const { // probability of winning, 0.0-1.0
        return m_prob.probability();
    }
    PerMille per_mille() const { return m_prob; }

private:
    Competitor m_competitor;
    std::string m_info;
    PerMille m_prob; // ‰ 0-1000
};

// An Event contains a number of CompetitorConfigurations.
class Event {
public:
    PerMille per_mille() const {
        // Note that if sum of all CompetitorConfiguration:per_mille() in an event is < 1000,
        // it means there could be an event with no winner: a tie.
        return std::accumulate(m_competitors.begin(), m_competitors.end(), PerMille{},
                               [](PerMille lhs, CompetitorConfiguration const& rhs) { return lhs + rhs.per_mille(); });
    }

private:
    std::vector<CompetitorConfiguration> m_competitors;
};

// A Parlay is a set of events, a coupon or card (in horse racing)
class Parlay {
public:
private:
    std::vector<Event> m_events;
};

#endif
