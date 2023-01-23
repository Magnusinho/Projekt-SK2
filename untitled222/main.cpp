#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <error.h>
#include <poll.h>
#include <unordered_set>
#include <signal.h>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>
#include <ctime>

// server socket
int servFd;
using json = nlohmann::json;
using namespace std;

string cat_country_path = "/home/pawcio/Pobrane/ans/country/";
string cat_city_path = "/home/pawcio/Pobrane/ans/city/";
string cat_animal_path = "/home/pawcio/Pobrane/ans/animal/";
string cat_job_path = "/home/pawcio/Pobrane/ans/jobs/";
string cat_plant_path = "/home/pawcio/Pobrane/ans/plant/";
string cat_color_path = "/home/pawcio/Pobrane/ans/color/";

struct Player{
    string nick;
    int points;
    int id;
    int room_id=0;
};
std::vector<Player> players;

struct Category{
    string name;
    string filePath;
};
vector<Category> categories;

Category color;
Category plant;
Category country;
Category city;
Category job;
Category animal;

struct Lobby{
    int id;
    string hostNick;
    vector<Player> players;
    vector<Category> categories;
    bool join;
    int rounds;
    vector<char> letters = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'X', 'M',
                            'N', 'O', 'P', 'R', 'S', 'T', 'U', 'W', 'Z'};
    json json_answers;
    json json_ans_uniq;
    bool operator==(const Lobby& other) const {
        return id == other.id;}

};
vector<Lobby> lobbies;

// data for poll
int descrCapacity = 16;
int descrCount = 1;
pollfd * descr;

json stop_game(json json_input){
    json json_output;
    json_output["action"] = "get_ans";
    json_output["status"] = true;
    return json_output.dump(-1);
}
void nickAdd(json json_input, int fd)
{
    Player player;
    player.nick = json_input["nick"];
    player.id = fd;
    player.points = 0;
    for(auto &play : players)
    {
        if(play.id == player.id)
        {
            return;
        }
    }
    players.push_back(player);
    return;
}

json showPlayersList(int roomid) {
    json json_message;
    json_message["status"] = true;
    json_message["action"] = "show_players_list";
    json player_list;
    for(auto& lobby : lobbies)
    {
        if(lobby.id == roomid)
        {
            for (auto& player : lobby.players) {
                player_list.push_back(player.nick);
            }
        }
    }
    json_message["players"] = player_list;
    return json_message.dump(-1);
}

void showPlayersListAll() {
    json json_message;
    json_message["status"] = true;
    json_message["action"] = "show_players_list";
    json player_list;
    cout << "ALL PLAYERS \n" <<player_list << endl;
    for (auto &player: players) {
        player_list.push_back(player.nick);
        cout<<player.nick<< "- ROOM ID: " << player.room_id <<endl;
    }

}

json addPlayerToLobby(json json_message) {
    json json_output;
    for (auto &l : lobbies) {
        if(json_message["room_id"] == l.id && l.join == true)
        {
            for (auto &p : players)
            {
                if(p.nick.compare(json_message["nick"]) == 0)
                {
                    p.room_id = json_message["room_id"];
                    l.players.push_back(p);
                    json_output["status"] = true;
                    break;
                }
            }
            break;
        }
        else if(json_message["room_id"] == l.id && l.join == false)
        {
            json_output["status"] = false;
        }
    }
    json_output["room_id"] = json_message["room_id"];
    json_output["action"] = "joined";
    json_output["role"] = "user";
    return json_output.dump(-1);
}

json showLobbiesList(json json_input) {
    json json_message;
    json_message["status"] = true;
    json_message["action"] = "show_rooms";
    json lobbies_list;
    for (auto& lobby : lobbies) {
        json temp;
        temp["host"] = lobby.hostNick;
        temp["id"] = lobby.id;
        temp["join"] = lobby.join;
        lobbies_list.push_back(temp);
    }
    json_message["rooms"] = lobbies_list;
    return json_message.dump(-1);
}
void showLobbiesAll()
{
    cout << "=======================" <<endl;
    for (auto& lobby : lobbies) {
       cout<<"["<<lobby.id<<"] - HOST: ["<< lobby.hostNick << "]";
        for(auto& player : lobby.players){
            cout<<"- ["<< player.nick << "] -";
        }
        cout<<endl;

    }
    cout << "=======================" <<endl;
}
json createLobby(json json_input){
    json json_message;
    Lobby lob;
    for (auto category : categories) {
        for(auto catname:json_input["categories"]){
            if(category.name == catname) {
                lob.categories.push_back(category);
            }
        }
    }
    lob.hostNick = json_input["host"];
    lob.join = true;
    lob.rounds = json_input["roundAmount"];
    int id = 1;
    for (auto lobby : lobbies)
    {
        if (lobby.id == id) id++;
        else break;
    }
    lob.id = id;
    for (auto& i : players ) {
        if (i.nick == json_input["host"]) {
            i.room_id = id;
            lob.players.push_back(i);
            break;
        }
    }
    lobbies.push_back(lob);
    json_message["status"] = true;
    json_message["action"] = "joined";
    json_message["role"] = "admin";
    json_message["room_id"] = id;
    return json_message.dump(-1);

}

json startRound(json json_input)
{
    json json_output;
    json_output["action"] = "start_round";
    json_output["status"] = false;
    for (auto& lobby : lobbies)
    {
        if(lobby.id == json_input["room_id"]) {
            if(lobby.join == true) lobby.join = false;
            if (lobby.rounds > 0) {
                lobby.rounds -= 1;
                lobby.join = false;
                srand(time(nullptr));
                int i;
                i = std::rand() % lobby.letters.size();
                char c = lobby.letters[i];
                if (c == 'X') {
                    json_output["letter"] = "Ł";
                } else json_output["letter"] = c;
                lobby.letters.erase(lobby.letters.begin() + i);
                json_output["status"] = true;
                for (auto category: lobby.categories) {
                    json_output["categories"].push_back(category.name);
                }
                break;
            }
            else{
                    json_output["action"] = "show_total_score";
                    json_output["status"] = true;
                    json_output["room_id"] = json_input["room_id"];
                    json jtemp;
                    for (auto lobby : lobbies)
                    {
                        if(lobby.id == json_input["room_id"]) {
                            for(auto player: lobby.players)
                            {
                                jtemp[player.nick] = player.points;
                            }
                            json_output["score"] = jtemp;
                            break;
                        }
                    }
            }
        }
    }
    return json_output.dump();
}

json showScores(json json_input)
{
    json json_output;
    json jtemp;
    json_output["room_id"] = json_input["room_id"];
    json_output["action"] = "show_total_score";
    for (auto lobby : lobbies)
    {
        if(lobby.id == json_input["room_id"]) {
            for(auto player: lobby.players)
            {
                jtemp[player.nick] = player.points;
            }
            json_output["score"] = jtemp;
            break;
        }
    }
    return json_output.dump(-1);
}

json sendAns(json json_input){
    json fake_json;
    fake_json = json_input["ans"];
    for(auto &lobby: lobbies){
        if(lobby.id == json_input["room_id"])
        {
            lobby.json_answers[json_input["nickname"]] = fake_json;
            for(auto &category:categories)
            {
                auto it = std::find_if(lobby.categories.begin(), lobby.categories.end(), [&](const Category& item){ return item.name == category.name; });
                if(fake_json[category.name].empty() && it != lobby.categories.end())
                {
                    lobby.json_ans_uniq[category.name] = fake_json[category.name];
                }
                else if(it != lobby.categories.end())
                {
                    lobby.json_ans_uniq[category.name].push_back(fake_json[category.name]);
                }
            }
            lobby.json_answers["letter"] = json_input["letter"];
        }
    }
    fake_json["action"] = "send_ans";
    fake_json["status"] = true;
    fake_json["room_id"] = json_input["room_id"];
    return fake_json.dump(-1);
}

json checkAns(json json_input, int room_id) {
    json json_output;
    int tempPoints = 0;
    json_output["action"] = "show_ans";
    json_output["status"] = true;
    json_output["room_id"] = room_id;
    json json_output_temp;
    json empty;
    for(auto &lobby_it :lobbies) {
        if (lobby_it.id == room_id) {
            for (auto &player: lobby_it.players) {
                for (json::iterator it = json_input.begin(); it != json_input.end(); ++it) {
                    if(it.key() == "letter")
                    {
                        continue;
                    }
                    if (it.key() == player.nick) {
                        for (json::iterator ansIt = it.value().begin(); ansIt != it.value().end(); ansIt++) {
                            for (auto &category: categories) {
                                if (category.name == ansIt.key()) {
                                    string finalPath = category.filePath;
                                    if(json_input["letter"] == "X")
                                    {
                                        finalPath += "L-2";
                                    }
                                    else finalPath += json_input["letter"];
                                    ifstream cat_file(finalPath);

                                    string answer;
                                    while (cat_file >> answer) {
                                        if (answer == json_input[it.key()][ansIt.key()]) {
                                            int count = 0;
                                            for(auto upAns : lobby_it.json_ans_uniq[ansIt.key()])
                                            {
                                                if(upAns == answer) count++;
                                            }
                                            if(count >1)
                                            {
                                                player.points += 5;
                                                tempPoints += 5;
                                            }
                                            else{
                                                player.points += 10;
                                                tempPoints += 10;
                                            }

                                            break;
                                        }
                                    }
                                    json_output["players"][it.key()][ansIt.key()]= json_input[it.key()][ansIt.key()];
                                }
                            }
                        }
                        json_output["players"][player.nick]["score"] = tempPoints;
                        tempPoints = 0;
                        json_output["players"][player.nick]["total_score"] = player.points;
                    }
                }
            }
        }
        lobby_it.json_answers = empty;
    }
    return json_output;
}

// handles SIGINT
void ctrl_c(int);

// sends data to clientFds excluding fd
void sendToAllBut(int fd, char * buffer, int count);
void sendToOne(int fd, char * buffer, int count);
void sendToLobby(int room_id,char * buffer, int count);
// converts cstring to port
uint16_t readPort(char * txt);

// sets SO_REUSEADDR
void setReuseAddr(int sock);

void eventOnServFd(int revents) {
    // Wszystko co nie jest POLLIN na gnieździe nasłuchującym jest traktowane
    // jako błąd wyłączający aplikację
    if(revents & ~POLLIN){
        error(0, errno, "Event %x on server socket", revents);
        ctrl_c(SIGINT);
    }

    if(revents & POLLIN){
        sockaddr_in clientAddr{};
        socklen_t clientAddrSize = sizeof(clientAddr);

        auto clientFd = accept(servFd, (sockaddr*) &clientAddr, &clientAddrSize);
        if(clientFd == -1) error(1, errno, "accept failed");

        if(descrCount == descrCapacity){
            // Skończyło się miejsce w descr - podwój pojemność
            descrCapacity<<=1;
            descr = (pollfd*) realloc(descr, sizeof(pollfd)*descrCapacity);
        }

        descr[descrCount].fd = clientFd;
        descr[descrCount].events = POLLIN|POLLRDHUP;
        descrCount++;

        printf("new connection from: %s:%hu (fd: %d)\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), clientFd);
    }
}
unordered_map<std::string, std::function<json(json)>> action_handlers = {
        {"show_rooms", &showLobbiesList},
        {"join", &addPlayerToLobby},
        {"start_round", &startRound},
        {"stop_game", &stop_game},
        {"send_ans",&sendAns},
        {"show_total_score",&showScores},
        {"create", createLobby}
};

json chooseAction(json json_input)
{
    string value = json_input["action"].get<string>();
    json json_err;
    json_err["status"] = false;
    json_err["action"] = json_input["action"];

    auto it = action_handlers.find(value);
    if (it != action_handlers.end()) {
        return it->second(json_input);
    }
    else {
        return json_err;
    }
}

void eventOnClientFd(int indexInDescr) {
    auto clientFd = descr[indexInDescr].fd;
    auto revents = descr[indexInDescr].revents;

    if (revents & POLLIN) {
        char buffer[1000];
        int count = read(clientFd, buffer, 1000);
        if (count < 1)
            revents |= POLLERR;
        else {
            for (int i = 1000; i > 0; i--) {
                if (buffer[i] == '}') {
                    buffer[i + 1] = '\0';
                    break;
                }
            }
            json json_message = json::parse(buffer);

            if (json_message["action"] == "show_rooms") {
                nickAdd(json_message, clientFd);
            }
            json json_output = chooseAction(json_message);
            std::string json_mess = json_output.get<std::string>();
            char message[1000];
            size_t pos = json_mess.rfind("}");
            int length = json_mess.copy(message, 1000);
            if (pos != std::string::npos) {
                length = pos + 1;
                message[length] = '\0';
            }
            if (json_message["action"] == "create") {
                sendToOne(clientFd, message, sizeof(message));
                json tempid = json::parse(message);
                json temp = showPlayersList(tempid["room_id"]);
                string json_mess2 = temp.get<std::string>();
                char message2[1000];
                size_t pos2 = json_mess2.rfind("}");
                int length2 = json_mess2.copy(message2, 1000);
                if (pos2 != std::string::npos) {
                    length2 = pos2 + 1;
                    message2[length2] = '\0';
                }
                sendToLobby(tempid["room_id"], message2, sizeof(message2));
                memset(message2, 0, sizeof(message2));

            } else if (json_message["action"] == "send_ans") {
                for (auto lobby: lobbies) {
                    if (lobby.id == json_message["room_id"]) {
                        if ((lobby.json_answers.size() - 1) == lobby.players.size()) {
                            json temp = checkAns(lobby.json_answers, lobby.id);
                            string json_mess2 = temp.dump(-1);
                            char message2[1000];
                            size_t pos2 = json_mess2.rfind("}");
                            int length2 = json_mess2.copy(message2, 1000);
                            if (pos2 != std::string::npos) {
                                length2 = pos2 + 1;
                                message2[length2] = '\0';
                            }
                            sendToLobby(json_message["room_id"], message2, sizeof(message2));
                            memset(message2, 0, sizeof(message2));
                            json nowy_json;
                            lobby.json_answers = nowy_json;
                            json temp2;
                            string temp3;
                            temp2["action"] = "start_round";
                            temp2["room_id"] = temp["room_id"];
                            temp = startRound(temp2).get<string>();
                            temp3 = temp.get<string>();
                            pos2 = temp3.rfind("}");
                            length2 = temp3.copy(message2, 1000);
                            if (pos2 != std::string::npos) {
                                length2 = pos2 + 1;
                                message2[length2] = '\0';
                            }
                            time_t start;
                            time(&start);
                            time_t end = start + 10;
                            while (time(NULL) < end) {
                            }
                            sendToLobby(json_message["room_id"], message2, sizeof(message2));
                            temp = json::parse(message2);
                            if (temp["action"] == "show_total_score") {
                                auto it = std::remove(lobbies.begin(), lobbies.end(), lobby);
                                lobbies.erase(it, lobbies.end());
                            }
                            memset(message2, 0, sizeof(message2));
                        }
                    }
                }
            } else if (json_message["action"] == "join") {
                sendToOne(clientFd, message, sizeof(message));
                string json_mess2 = showPlayersList(json_message["room_id"]).get<std::string>();
                char message2[1000];
                size_t pos2 = json_mess2.rfind("}");
                int length2 = json_mess2.copy(message2, 1000);
                if (pos2 != std::string::npos) {
                    length2 = pos2 + 1;
                    message2[length2] = '\0';
                }
                sendToLobby(json_message["room_id"], message2, sizeof(message2));
                memset(message2, 0, sizeof(message2));
            } else if (json_message["action"] == "start_round" || json_message["action"] == "stop_game") {
                sendToLobby(json_message["room_id"], message, sizeof(message));
            } else {
                sendToOne(clientFd, message, sizeof(message));
            }
            memset(message, 0, sizeof(message));
        }
        memset(buffer, 0, sizeof(buffer));
    }

    if (revents & ~POLLIN) {
        printf("removing %d\n", clientFd);
        players.erase(std::remove_if(players.begin(), players.end(), [clientFd](const Player &player) {
            return player.id == clientFd;
        }), players.end());
        for (auto &lobby: lobbies) {
            for (auto &player: lobby.players) {
                if (player.id == clientFd && lobby.hostNick == player.nick) {
                    lobbies.erase(
                            std::remove_if(lobbies.begin(), lobbies.end(), [clientFd, lobby](const Lobby &lobby2) {
                                return lobby.id == lobby2.id;
                            }), lobbies.end());
                } else if (lobby.hostNick != player.nick && player.id == clientFd) {
                    lobby.players.erase(
                            std::remove_if(lobby.players.begin(), lobby.players.end(),
                                           [clientFd](const Player &player) {
                                               return player.id == clientFd;
                                           }), lobby.players.end());
                }
            }
        }
            descr[indexInDescr] = descr[descrCount - 1];
            descrCount--;
            shutdown(clientFd, SHUT_RDWR);
            close(clientFd);
        }
    }




int main(int argc, char ** argv){

    players.reserve(50);
    categories.reserve(10);
    lobbies.reserve(20);

    color.name = "color";
    plant.name = "plant";
    country.name = "country";
    city.name = "city";
    job.name = "jobs";
    animal.name = "animal";

    color.filePath = cat_color_path;
    plant.filePath = cat_plant_path;
    country.filePath = cat_country_path;
    city.filePath = cat_city_path;
    job.filePath = cat_job_path;
    animal.filePath = cat_animal_path;

    categories.push_back(color);
    categories.push_back(plant);
    categories.push_back(country);
    categories.push_back(city);
    categories.push_back(job);
    categories.push_back(animal);

    // get and validate port number
    auto port = 3737;

    // create socket
    servFd = socket(AF_INET, SOCK_STREAM, 0);
    if(servFd == -1) error(1, errno, "socket failed");

    // graceful ctrl+c exit
    signal(SIGINT, ctrl_c);
    // prevent dead sockets from throwing pipe errors on write
    signal(SIGPIPE, SIG_IGN);

    setReuseAddr(servFd);

    // bind to any address and port provided in arguments
    sockaddr_in serverAddr{.sin_family=AF_INET, .sin_port=htons((short)port), .sin_addr={INADDR_ANY}};
    int res = bind(servFd, (sockaddr*) &serverAddr, sizeof(serverAddr));
    if(res) error(1, errno, "bind failed");

    // enter listening mode
    res = listen(servFd, 1);
    if(res) error(1, errno, "listen failed");

    descr = (pollfd*) malloc(sizeof(pollfd)*descrCapacity);

    descr[0].fd = servFd;
    descr[0].events = POLLIN;

    while(true){
        int ready = poll(descr, descrCount, -1);
        if(ready == -1){
            error(0, errno, "poll failed");
            ctrl_c(SIGINT);
        }

        for(int i = 0 ; i < descrCount && ready > 0 ; ++i){
            if(descr[i].revents){
                if(descr[i].fd == servFd)
                    eventOnServFd(descr[i].revents);
                else
                    eventOnClientFd(i);
                ready--;
            }
        }
    }
}

uint16_t readPort(char * txt){
    char * ptr;
    auto port = strtol(txt, &ptr, 10);
    if(*ptr!=0 || port<1 || (port>((1<<16)-1))) error(1,0,"illegal argument %s", txt);
    return port;
}

void setReuseAddr(int sock){
    const int one = 1;
    int res = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    if(res) error(1,errno, "setsockopt failed");
}

void ctrl_c(int){
    for(int i = 1 ; i < descrCount; ++i){
        shutdown(descr[i].fd, SHUT_RDWR);
        close(descr[i].fd);
    }
    close(servFd);
    printf("Closing server\n");
    exit(0);
}

void sendToAllBut(int fd, char * buffer, int count){
    int i = 1;
    while(i < descrCount){
        int clientFd = descr[i].fd;
        if(clientFd == fd) {
            i++;
            continue;
        }
        int res = write(clientFd, buffer, count);
        if(res!=count){
            printf("removing %d\n", clientFd);
            shutdown(clientFd, SHUT_RDWR);
            close(clientFd);
            descr[i] = descr[descrCount-1];
            descrCount--;
            continue;

        }
        i++;
    }
}
void sendToOne(int fd, char * buffer, int count){
    int i = 1;
    while(i < descrCount){
        int clientFd = descr[i].fd;
        if(clientFd == fd) {
            int res = write(clientFd, buffer, count);
            i++;
            if(res!=count){
                printf("removing %d\n", clientFd);
                shutdown(clientFd, SHUT_RDWR);
                close(clientFd);
                descr[i] = descr[descrCount-1];
                descrCount--;
            }
            break;
        }
        i++;
    }
}

void sendToLobby(int room_id,char * buffer, int count) {
    int i = 0;
    json jsonTemp;
    for (auto& lobby: lobbies){
        if (lobby.id == room_id){
            for (auto& player: lobby.players) {
                if (player.room_id == room_id) {
                    jsonTemp["players"].push_back(player.id);
                    i++;
                }
            }
            break;
        }
    }
    int j;
    for(j = 0; j < i; j++) {
        int clientFd = jsonTemp["players"][j];
        int res = write(clientFd, buffer, count);
        if (res != count) {
            printf("removing %d\n", clientFd);
            shutdown(clientFd, SHUT_RDWR);
            close(clientFd);
            descr[i] = descr[descrCount - 1];
            descrCount--;
        }
    }
}
