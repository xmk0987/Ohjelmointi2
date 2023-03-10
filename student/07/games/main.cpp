/*  COMP.CS.100 Project 2: GAME STATISTICS
 * ===============================
 * EXAMPLE SOLUTION
 * ===============================
 *
 * Acts as a game statistics with n commands:
 * ALL_GAMES - Prints all known game names
 * GAME <game name> - Prints all players playing the given game
 * ALL_PLAYERS - Prints all known player names
 * PLAYER <player name> - Prints all games the given player plays
 * ADD_GAME <game name> - Adds a new game
 * ADD_PLAYER <game name> <player name> <score> - Adds a new player with the
 * given score for the given game, or updates the player's score if the player
 * already playes the game
 * REMOVE_PLAYER <player name> - Removes the player from all games
 *
 *  The data file's lines should be in format game_name;player_name;score
 * Otherwise the program execution terminates instantly (but still gracefully).
 *
 * Program Author
 * Name: Onni Vitikainen
 * Student number: H292259
 * Username: ftonvi
 * E-mail: onni.vitikainen@tuni.fi
 *
 * */
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <cctype>
#include <algorithm>

// Creating struct for player
struct Player{
    std::string player_name;
    int points;
};

// Used data structure to shorten function parameters
using data_structure = std::map<std::string, std::vector<Player>>;

// Casual split func, if delim char is between "'s, ignores it.
std::vector<std::string> split( const std::string& str, char delim = ';' )
{
    std::vector<std::string> result = {""};
    bool inside_quatation = false;
    for ( auto current_char : str )
    {
        if ( current_char == '"' )
        {
            inside_quatation = not inside_quatation;
        }
        else if ( current_char == delim and not inside_quatation )
        {
            result.push_back("");
        }
        else
        {
            result.back().push_back(current_char);
        }
    }
    if ( result.back() == "" )
    {
        result.pop_back();
    }
    return result;
}

// Sorts in alphabetical order
bool sort_alphabetical(std::string a, std::string b){ return a<b;}

// Checks input in file
bool check_input(std::vector <std::string>& parts){
    if (parts.size() != 3){
        return false;
    }
    for (auto& part : parts){
        if (part == ""){
            return false;
        }
    }
    return true;
}

// Checks if game with given name exist in data structure, returns true if yes and false if no
bool check_games(data_structure games, std::string game_name){
    if(games.find(game_name) == games.end()){
        return false;
    }
    else{
        return true;
    }
}

// Checks if given player exists in data structure, returns true if yes and false if no
bool check_player(data_structure games, std::string player_name){
    for (auto element : games){
        for ( int i = 0; i < int(games.at(element.first).size()); ++i){
            if(player_name == games.at(element.first).at(i).player_name){
                return true;
            }
        }
    }
        return false;
}

// Turns all character in string to uppercase and returns string
std::string touppercase(std::string string){
    std::for_each(string.begin(),string.end(), [] (char & c){ c = ::toupper(c);});
    return string;
}

// Prints all games in ASCII order
void all_games(data_structure games){
    std::cout << "All games in alphabetical order:"<<std::endl;
    for (auto element : games){
        std::cout << element.first << std::endl;
    }
}

// Prints vector separated with wanted character used in game function
template <typename T>
void print_vector(const std::vector<T> & vec, std::string sep=" ")
{
    for(unsigned int i = 0; i < vec.size();i++)
    {
        if (i == vec.size()-1){
            std::cout<<vec.at(i)<<std::endl;
        }
        else{
            std::cout<<vec.at(i)<< sep;
        }
    }
}

// Prints all players in a given game and their points in ascending order.
void game(data_structure games, std::string name){
    std::cout << "Game " << name << " has these scores and players, listed in ascending order:" << std::endl;
    std::map<int , std::vector<std::string>> for_print;

    for (unsigned int i = 0; i < games.at(name).size(); i++){
        if(for_print.find(games.at(name).at(i).points) == for_print.end()){
            for_print.insert({games.at(name).at(i).points, {games.at(name).at(i).player_name}});
        }
        else{
            for_print.at(games.at(name).at(i).points).push_back(games.at(name).at(i).player_name);
        }
    }

    for (auto element : for_print){
        std::cout << element.first << " : ";
        print_vector(for_print.at(element.first), ", ");
    }
}

// Prints all players in alphabetical order
void all_players(data_structure games){
    std::cout << "All players in alphabetical order:" << std::endl;
    std::vector <std::string> players;
    for (auto element : games){
        std::vector<Player> inVect = element.second;
        for (unsigned j=0; j< inVect.size(); j++){
            players.push_back(inVect.at(j).player_name);
        }
    }
    std::sort(players.begin(), players.end(), sort_alphabetical);
    players.erase(unique(players.begin(), players.end()),players.end());
    for( auto it : players){
        std::cout << it << std::endl;
    }
}

// Prints all games a given player is playing
void player_games(data_structure games, std::string player_name){
   std::cout << "Player "<< player_name << " playes the following games:"<< std::endl;
   std::vector <std::string > games_played;
   for (auto element : games){
       std::vector<Player> inVect = element.second;
       for (unsigned j=0; j< inVect.size(); j++){
           if (inVect.at(j).player_name == player_name){
               games_played.push_back(element.first);
           }
       }
   }
   std::sort(games_played.begin(),games_played.end(), sort_alphabetical);
   for(std::string& i: games_played){
       std::cout << i << std::endl;
   }
}

// Checks given command according to command
bool check_command(std::vector <std::string> parts, unsigned int size, std::string command, data_structure games){
    if (parts.size() < size){
        std::cout << "Error: Invalid input." << std::endl;
        return false;
    }
    if (command == "GAME"){
        if(games.find(parts.at(1)) == games.end()){
            std::cout << "Error: Game could not be found." << std::endl;
            return false;
        }
    }
    if (command == "PLAYER"){
        if(!check_player(games, parts.at(1))){
            std::cout << "Error: Player could not be found." << std::endl;
            return false;
        }
    }
    return true;
}

// Adds new game to data structure if not already in it
void add_game(data_structure& games, std::string game_name){
    if(games.find(game_name) == games.end()){
        games[game_name]= {};
        std::cout << "Game was added." << std::endl;
    }
    else{
        std::cout << "Error: Already exists." << std::endl;
    }
}

// Removes a player from all games
void remove_player(data_structure& games, std::string player_name){
    for(std::map <std::string,std::vector < Player>>::iterator it = games.begin();
           it != games.end(); ++it){
            games.at(it->first).erase(std::remove_if(games.at(it->first).begin(),
                                                     games.at(it->first).end(),
                                                     [&](Player const& player){
                return player.player_name == player_name;
            }), games.at(it->first).end());
        }
    std::cout << "Player was removed from all games." << std::endl;
}

// Adds new player to a game if already exist update his score
void add_player(data_structure& games,std::string game_name, std::string player_name, int score){
    if(!check_player(games, player_name)){
        struct Player player = {player_name, score};
        games.at(game_name).push_back(player);
        std::cout<< "Player was added." << std::endl;
    }
    else{
        for(int i = 0; games.at(game_name).size(); i ++){
            if (games.at(game_name).at(i).player_name == player_name){
                games.at(game_name).at(i).points = score;
                std::cout<< "Player was added." << std::endl;
                break;
            }
        }
    }
}

// Programs interface, running the program
void running_program(data_structure games){
    while(true){
        std::string line;
        std::cout << "games> ";
        getline(std::cin, line);
        std::vector<std::string> parts = split(line,' ');
        // All commands with error check function when needed, all commands perform own action
        if(!check_command(parts, 1, "", games)){
            continue;
        }
        else if (touppercase(parts.at(0)) == "QUIT"){
            break;
        }
        else if (touppercase(parts.at(0)) == "ALL_GAMES"){
            all_games(games);
            continue;
        }
        else if (touppercase(parts.at(0)) == "GAME"){
            if (!check_command(parts, 2, "GAME",games)){
                continue;
            }
            game(games, parts.at(1));
            continue;
        }
        else if (touppercase(parts.at(0)) == "ALL_PLAYERS"){
            all_players(games);
            continue;
        }
        else if (touppercase(parts.at(0)) == "PLAYER"){
            if (!check_command(parts,2, "PLAYER", games)){
                continue;
            }
            player_games(games, parts.at(1));
            continue;
        }
        else if (touppercase(parts.at(0)) == "ADD_GAME"){
            if (!check_command(parts, 2, "ADD_GAME",games)){
                continue;
            }
            add_game(games, parts.at(1));
            continue;
        }
        else if(touppercase(parts.at(0)) == "ADD_PLAYER"){
            if(!check_command(parts,4, "GAME",games)){
                continue;
            }
            add_player(games, parts.at(1), parts.at(2), stoi(parts.at(3)));
            continue;
        }
        else if(touppercase(parts.at(0))== "REMOVE"){
            if(!check_command(parts, 2, "PLAYER", games)){
                continue;
            }
            remove_player(games, parts.at(1));
            continue;
        }
        else{
            std::cout<<"Error: Invalid input." << std::endl;
        }
    }
}

// Main function reading file to data structure with error check and runs program and ends if
// error
int main()
{
    std::string input_file = "";
    std::cout << "Give a name for input file: ";
    getline(std::cin, input_file);
    std::ifstream input_creature(input_file);
    if (not input_creature){
        std::cout << "Error: File could not be read." << std::endl;
        return EXIT_FAILURE;
    }
    std::string line;

    std::map<std::string, std::vector<Player>> games;
    while(getline(input_creature,line)){
        std::vector<std::string> parts = split(line);
        if (!check_input(parts)){
            std::cout<<"Error: Invalid format in file."<<std::endl;
            return EXIT_FAILURE;
        }

        std::string game_name = parts.at(0);
        std::string player_name = parts.at(1);
        int points = std::stoi(parts.at(2));
        struct Player player = {player_name, points};

        if(games.find(game_name) == games.end()){
            games.insert({game_name, {player}});
        }
        else{
            games.at(game_name).push_back(player);
        }
    }
    input_creature.close();

    running_program(games);
    return EXIT_SUCCESS;
}
