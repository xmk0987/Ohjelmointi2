/* Hitori
 *
 * Kuvaus:
 *   Ohjelma toteuttaa Hitori-pelin. Pelissä on peliruudukko kooltaan
 * 5 x 5. Kukin ruutu sisältää jonkin numeroista 1-5. Vaaka- ja
 * pystyriveillä voi aluksi olla useita samoja numeroita, mutta
 * tarkoituksena on poistaa numeroita niin, että kullakin vaaka- ja
 * pystyrivillä on kutakin numeroa vain yksi tai ei yhtään. Kuitenkaan
 * vierekkäisten ruutujen numeroita ei saa poistaa, eikä mikään jäljelle
 * jäävä numero (ruutu) saa jäädä irralleen muista, eli sen ympäriltä
 * (yläpuolelta, alapuolelta, vasemmalta, oikealta) ei saa poistaa
 * kaikkia numeroita.
 *   Aluksi käyttäjältä kysytään, täytetäänkä peliruudukko satunnaisesti
 * arvottavilla luvuilla 1-5 vai käyttäjän valitsemilla 25 luvulla.
 * Ensimmäisessä vaihtoehdossa käyttäjältä kysytään satunnaisluku-
 * generaattorin siemenlukua ja jälkimmäisessä häntä pyydetään syättämään
 * 25 lukua.
 *   Joka kierroksella käyttäjältä kysytään poistettavan numeron
 * koordinaatteja eli kahta lukua. Peli päättyy pelaajan voittoon,
 * jos peliruudukon kaikilla vaaka- ja pystyriveillä esiintyy kukin
 * numero 1-5 korkeintaan kerran. Peli päättyy pelaajan häviään,
 * jos hän poistaa jo poistetun numeron viereisen numeron tai jos
 * jokin numero jää irralleen muista.
 *   Ohjelma tarkistaa annetut koordinaatit. Koordinaattien pitää olla
 * peliruudukon sisällä, ja niiden osoittaman ruudun pitää sisältää
 * numero, eli jo poistettua numeroa ei voi poistaa toiseen kertaan.
 *   Pelin päättyessä kerrotaan, voittiko vai hävisikä pelaaja.
 *
 * Ohjelman kirjoittaja
 * Nimi: Onni Vitikainen
 * Opiskelijanumero: H292259
 * Käyttäjätunnus: ftonvi
 * E-Mail: onni.vitikainen@tuni.fi
 *
 * Huomioita ohjelmasta ja sen toteutuksesta:
 *
 * */





#include <iostream>
#include <vector>
#include <random>
#include <algorithm>


using namespace std;

const unsigned int BOARD_SIDE = 5;
const unsigned char EMPTY = ' ';

using Game_board_type = vector<vector<int>>;

// Muuttaa annetun numeerisen merkkijonon vastaavaksi kokonaisluvuksi
// (kutsumalla stoi-funktiota).
// Jos annettu merkkijono ei ole numeerinen, palauttaa nollan.
//
// Converts the given numeric string to the corresponding integer
// (by calling stoi).
// If the given string is not numeric, returns zero.
unsigned int stoi_with_check(const string& str)
{
    bool is_numeric = true;
    for(unsigned int i = 0; i < str.length(); ++i)
    {
        if(not isdigit(str.at(i)))
        {
            is_numeric = false;
            break;
        }
    }
    if(is_numeric)
    {
        return stoi(str);
    }
    else
    {
        return 0;
    }
}


// Splits the given string str into parts separated by the given delimiter
// and returns the parts in a vector.
// If the delimiter appears inside quatation, ignores delimiter,
// i.e. does not split quoted text.
std::vector<std::string> split_ignoring_quoted_delim(const std::string& str,
                                                     char delimiter)
{
    std::vector<std::string> result = {""};
    bool inside_quotation = false;
    for(char current_char : str)
    {
        if(current_char == '"')
        {
            inside_quotation = not inside_quotation;
        }
        else if(current_char == delimiter and not inside_quotation)
        {
            result.push_back("");
        }
        else
        {
            result.back().push_back(current_char);
        }
    }
    if(result.back() == "")
    {
        result.erase(--result.end());
    }
    return result;
}


// Prints the game board with row and column numbers.
void print(const Game_board_type& gameboard)
{
    cout << "=================" << endl;
    cout << "|   | 1 2 3 4 5 |" << endl;
    cout << "-----------------" << endl;
    for(unsigned int i = 0; i < BOARD_SIDE; ++i)
    {
        cout << "| " << i + 1 << " | ";
        for(unsigned int j = 0; j < BOARD_SIDE; ++j)
        {
            if(gameboard.at(i).at(j) == 0)
            {
                cout << EMPTY << " ";
            }
            else
            {
                cout << gameboard.at(i).at(j) << " ";
            }
        }
        cout << "|" << endl;
    }
    cout << "=================" << endl;
}


// Starts the game, asks for user input and fills the game_board vector with random numbers or input
// returns filled vectors
Game_board_type start_game(){

    Game_board_type gameboard;

    while (true){
        cout << "Select start (R for random, I for input): ";
        string input;
        getline(cin, input);
        vector<string> parts = split_ignoring_quoted_delim(input, ' ');
        string command = parts.at(0);
        if(command == "i" or command == "I"){                
                string line;
                cout << "Input: ";
                getline(cin, line);

                vector<string> parts = split_ignoring_quoted_delim(line, ' ');

                // Saves the input to vector as rows by jumping index 5
                int i = 0;               
                vector<int> v;
                while (true){
                    if (i == 25){
                        break;
                    }
                    v.push_back(stoi_with_check(parts.at(i)));
                    v.push_back(stoi_with_check(parts.at(i+1)));
                    v.push_back(stoi_with_check(parts.at(i+2)));
                    v.push_back(stoi_with_check(parts.at(i+3)));
                    v.push_back(stoi_with_check(parts.at(i+4)));
                    i += 5;
                    gameboard.push_back(v);
                    v.clear();
                }

                return gameboard;
                }

        else if (command == "r" or command == "R"){
            cout << "Enter seed value: ";
            int seed;
            cin >> seed;
            default_random_engine gen(seed);

            uniform_int_distribution<int> distr(1, 5);
            for (int i = 0; i < 5; i++){
                vector<int> v;
                int count = 0;
                while (true){
                    if (count == 5)
                    {
                        break;
                    }
                    else{
                        v.push_back(distr(gen));
                        count += 1;
                    }
                }
                gameboard.push_back(v);
                v.clear();
            }
            cin.ignore(1000, '\n');
            return gameboard;
        }
        else{
            continue;
        }
    }
}


// Checks if input is out of board or for other errors in input
bool input_check(Game_board_type gameboard,string x, string y){

    if (stoi_with_check(x) == 0 or stoi_with_check(y) == 0){
        cout << "Out of board" << endl;
        return 0;
    }

    else if(stoi_with_check(x) > 5 or stoi_with_check(x) < 1){
        cout << "Out of board" << endl;
        return 0;
    }
    else if(stoi_with_check(y) > 5 or stoi_with_check(y) < 1){
        cout << "Out of board" << endl;
        return 0;
    }
    else if(gameboard.at(stoi_with_check(y)-1).at(stoi_with_check(x)-1) == 0){
        cout << "Already removed" << endl;
        return 0;
    }
    else{
        return 1;
    }

}


// Checks if player loses the game with input and ends game if so
bool check_for_lose(Game_board_type gameboard){

    Game_board_type check_horizontal;
    check_horizontal.clear();
    vector<int> v;
    for (int j = 0; j < 5; j++){
        for (int i = 0; i < 5; i++){
            v.push_back(gameboard.at(i).at(j));
        }
        check_horizontal.push_back(v);
        v.clear();
    }

    //Checks if there are 2 empty spaces vertically next to each other
    for (int j = 0; j < 5;j++){
        for (int i = 0; i < 4; i++){
            if (gameboard.at(j).at(i) == 0 and gameboard.at(j).at(i+1) == 0){
                return 0;
            }
        }
    }


    //Checks if there are 2 empty spaces horizontally next to each other
    for (int j = 0; j < 5;j++){
        for (int i = 0; i < 4; i++){
            if (check_horizontal.at(j).at(i) == 0 and check_horizontal.at(j).at(i+1) == 0){
                return 0;
            }
        }
    }

    // Checks if number gets surrounded by empty spaces
    // (En ollu varma pitääkö tässä käyttää myös tätä vektori vektori rakennetta niin käytin sitä
    // mutta en keksinyt tätä optimaalisempaa tapaa niin tuli tosi pitkäks muuten varmaa oisin tehnyt
    // matriisi tyyliin, mut tosiaan en tiennyt oliko pakko käyttää rakennetta)
    for (int i = 0; i < 5; i++){
        if (i == 0){
            for (int j = 0; j < 5; j++){
                if (j== 0){
                   if (gameboard.at(i).at(j+1) == 0 and gameboard.at(i+1).at(j)==0) {
                       return 0;
                   }
                }
                if (j == 4){
                    if (gameboard.at(i).at(j-1) == 0 and gameboard.at(i+1).at(j)==0) {
                        return 0;
                    }
                }
                if (j == 1 || j == 2 || j==3){
                    if (gameboard.at(i).at(j-1) == 0 and gameboard.at(i+1).at(j)==0
                            and gameboard.at(i).at(j+1)==0) {
                        return 0;
                    }
                }
            }
        }

        if(i == 4){
            for (int j = 0; j< 5;j++){
                if (j== 0){
                   if (gameboard.at(i).at(j+1) == 0 and gameboard.at(i-1).at(j)==0) {
                       return 0;
                   }
                }
                if (j == 4){
                    if (gameboard.at(i).at(j-1) == 0 and gameboard.at(i-1).at(j)==0) {
                        return 0;
                    }
                }
                if (j == 1 || j == 2 || j==3){
                    if (gameboard.at(i).at(j-1) == 0 and gameboard.at(i-1).at(j)==0
                            and gameboard.at(i).at(j+1)==0) {
                        return 0;
                    }
                }
            }
        }

        if (i == 1 || i == 2|| i ==3 ){
            for (int j = 0; j< 5;j++){
                if (j== 0){
                   if (gameboard.at(i).at(j+1) == 0 and gameboard.at(i-1).at(j)==0
                           and gameboard.at(i+1).at(j)==0) {
                       return 0;
                   }
                }
                if (j == 4){
                    if (gameboard.at(i).at(j-1) == 0 and gameboard.at(i-1).at(j)==0
                            and gameboard.at(i+1).at(j)==0) {
                        return 0;
                    }
                }
                if (j == 1 || j == 2 || j==3){
                    if (gameboard.at(i).at(j-1) == 0 and gameboard.at(i-1).at(j)==0
                            and gameboard.at(i).at(j+1)==0 and gameboard.at(i+1).at(j)==0) {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

// Checks for win condition. If game not lost if all rows
// and columns only contain max 1 of each number win
bool check_for_win(Game_board_type gameboard){
    Game_board_type check_horizontal;
    check_horizontal.clear();
    vector<int> v;
    for (int j = 0; j < 5; j++){
        for (int i = 0; i < 5; i++){
            v.push_back(gameboard.at(i).at(j));
        }
        check_horizontal.push_back(v);
        v.clear();
    }

    // Checks vertical rows for dubplicates of each possible number and if
    // no duplicates total grows
    int vertical_counter = 0;
    int vertical_total = 0;
    for (int i = 0; i < 5; i++){
        for(int j = 1; j<6;j++){
            if (count(gameboard.at(i).begin(), gameboard.at(i).end(), j) < 2){
                vertical_counter += 1;
            }
        }

        if (vertical_counter == 5){
            vertical_total += 1;
            vertical_counter = 0;
        }
        else{
            vertical_counter = 0;
        }
    }

    // Checks horizontal rows for dubplicates of each possible number and if
    // no duplicates total grows
    int horizontal_counter = 0;
    int horizontal_total = 0;
    for (int i = 0; i < 5; i++){
        for(int j = 1; j<6;j++){
            if (count(check_horizontal.at(i).begin(), check_horizontal.at(i).end(), j) < 2){
                horizontal_counter += 1;
            }
        }
        if (horizontal_counter == 5){
            horizontal_total += 1;
            horizontal_counter = 0;
        }
        else{
            horizontal_counter = 0;
        }
    }
    // Checks if both rows and columns didnt have duplicates
    if (vertical_total == 5 and horizontal_total == 5){
        return 0;
    }
    return 1;

}


// Taking commands and executing actions according
void running_game(Game_board_type gameboard){


    while (true){
        string line;
        cout << "Enter removable element (x, y): " ;

        getline(cin, line);
        vector<string> parts = split_ignoring_quoted_delim(line, ' ');
        //Quits program with command q or Q
        if (parts.at(0) == "q" or parts.at(0) == "Q"){
            cout << "Quitting" << endl;
            break;
        }

        // Error check for input
        else if (input_check(gameboard, parts.at(0),parts.at(1)) == 0){
            continue;
        }

        // Removes tile according to coordinate and prints changed board
        else{
            int x = stoi_with_check(parts.at(0));
            int y = stoi_with_check(parts.at(1));
            gameboard.at(y-1).at(x-1) = 0;
            print(gameboard);

            if (check_for_lose(gameboard) == 0){
                cout << "You lost" << endl;
                break;
            }

            if (check_for_win(gameboard) == 0){
                cout << "You won" << endl;
                break;
            }
        }
    }
}


int main()
{
    Game_board_type gameboard = start_game();
    print(gameboard);
    running_game(gameboard);

    return 0;
}
