#include <iostream>
#include <string>
#include <vector>

// Component Class
class Player {
private:
    std::string name;
    int number;
public:
    Player(std::string n, int num) : name(n), number(num) {
        std::cout << "Player " << name << " created." << std::endl;
    }
    std::string get_info() const {
        return name + " (#" + std::to_string(number) + ")";
    }
    ~Player() {
        std::cout << "Player " << name << " destroyed." << std::endl;
    }
};

// Container Class (using Composition)
class Team {
private:
    std::string team_name;
    // Composition: Players are value members, owned by the Team.
    std::vector<Player> roster; 
    
public:
    Team(std::string name, const std::vector<Player>& initial_roster) 
        : team_name(name), 
          roster(initial_roster) // Players are deep-copied into the vector
    {
        std::cout << "\nTeam " << team_name << " formed." << std::endl;
    }

    void list_roster() const {
        std::cout << "\n--- " << team_name << " Roster ---" << std::endl;
        for (const auto& player : roster) {
            std::cout << player.get_info() << std::endl;
        }
    }
    
    ~Team() {
        std::cout << "\nTeam " << team_name << " disbanded." << std::endl;
        // All Player destructors are automatically called by vector's destructor
    }
};

int main() {
    // Temporary list of players (these will be copied)
    std::vector<Player> initial = {
        Player("Mike", 10), 
        Player("Sara", 7)
    };

    Team champions("The Mavericks", initial);
    champions.list_roster();
    
    // When 'champions' is destroyed, the players within its roster are also destroyed.
    return 0;
}