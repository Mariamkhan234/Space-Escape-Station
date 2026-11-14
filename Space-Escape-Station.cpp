
#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <vector>
using namespace std;

// Linked List Node for Inventory
class ItemNode {
public:
    string item;
    ItemNode* next;

    ItemNode(string name) {
        item = name;
        next = nullptr;
    }
};

// Inventory using Linked List
class Inventory {
private:
    ItemNode* head;

public:
    Inventory() {
        head = nullptr;
    }

    void addItem(string name) {
        if (hasItem(name)) {
            return;
        }

        ItemNode* newNode = new ItemNode(name);

        if (head == nullptr) {
            head = newNode;
        }
        else {
            ItemNode* temp = head;

            while (temp->next != nullptr) {
                temp = temp->next;
            }

            temp->next = newNode;
        }

        cout << name << " added to inventory!\n";
    }

    bool hasItem(string name) {
        ItemNode* temp = head;

        while (temp != nullptr) {
            if (temp->item == name) {
                return true;
            }

            temp = temp->next;
        }

        return false;
    }

    void display() {
        if (head == nullptr) {
            cout << "\nYour inventory is empty.\n";
            return;
        }

        cout << "\n------ INVENTORY ------\n";

        ItemNode* temp = head;

        while (temp != nullptr) {
            cout << "- " << temp->item << endl;
            temp = temp->next;
        }
    }

    ~Inventory() {
        ItemNode* temp;

        while (head != nullptr) {
            temp = head;
            head = head->next;
            delete temp;
        }
    }
};

// Binary Tree Node for Rooms
class RoomNode {
public:
    string roomName;
    string description;

    RoomNode* left;
    RoomNode* right;
    RoomNode* parent;

    RoomNode(string name, string desc) {
        roomName = name;
        description = desc;

        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
};

// Main Game Class
class SpaceEscapeGame {
private:
    RoomNode* root;
    RoomNode* currentRoom;

    stack<RoomNode*> roomHistory;
    queue<string> missions;
    vector<string> visitedRooms;

    Inventory inventory;

    string playerName;

    bool accessCard;
    bool powerRestored;
    bool escapeKey;
    bool escaped;

    bool storageBonus;
    bool medicalBonus;

    int score;

    // Create the Binary Tree of Rooms
    void createRooms() {
        root = new RoomNode(
            "Control Center",
            "The main control room of the abandoned space station."
        );

        root->left = new RoomNode(
            "Research Lab",
            "A laboratory containing important station information."
        );

        root->right = new RoomNode(
            "Engineering Room",
            "The room responsible for the station's power supply."
        );

        root->left->parent = root;
        root->right->parent = root;

        root->left->left = new RoomNode(
            "Storage Room",
            "An old storage room containing useful supplies."
        );

        root->left->right = new RoomNode(
            "Medical Room",
            "A room used by the station's medical team."
        );

        root->right->left = new RoomNode(
            "Security Room",
            "A restricted room containing important security equipment."
        );

        root->right->right = new RoomNode(
            "Escape Pod",
            "The final destination. Use it to escape the station."
        );

        root->left->left->parent = root->left;
        root->left->right->parent = root->left;

        root->right->left->parent = root->right;
        root->right->right->parent = root->right;
    }

    // Add Missions to Queue
    void addMissions() {
        missions.push("Find the Access Card");
        missions.push("Restore the Station Power");
        missions.push("Find the Escape Key");
        missions.push("Reach the Escape Pod");
    }

    // Display Pending Missions
    void displayMissions() {
        cout << "\n------ PENDING MISSIONS ------\n";

        if (missions.empty()) {
            cout << "All missions completed!\n";
            return;
        }

        queue<string> temp = missions;

        int number = 1;

        while (!temp.empty()) {
            cout << number << ". " << temp.front() << endl;

            temp.pop();
            number++;
        }
    }

    // Update Missions
    void updateMissions() {
        queue<string> temp;

        while (!missions.empty()) {
            string mission = missions.front();

            missions.pop();

            if (mission == "Find the Access Card" && accessCard) {
                cout << "\nMission completed: Find the Access Card!\n";
                score += 20;
            }

            else if (mission == "Restore the Station Power" &&
                     powerRestored) {
                cout << "\nMission completed: Restore the Station Power!\n";
                score += 30;
            }

            else if (mission == "Find the Escape Key" && escapeKey) {
                cout << "\nMission completed: Find the Escape Key!\n";
                score += 30;
            }

            else if (mission == "Reach the Escape Pod" &&
                     currentRoom->roomName == "Escape Pod" &&
                     accessCard && powerRestored && escapeKey) {

                cout << "\nMission completed: Reach the Escape Pod!\n";
                score += 50;
            }

            else {
                temp.push(mission);
            }
        }

        missions = temp;
    }

    // Display Current Room
    void showRoom() {
        cout << "\nCurrent Room: " << currentRoom->roomName << endl;
        cout << currentRoom->description << endl;

        cout << "\nAvailable Rooms:\n";

        if (currentRoom->left != nullptr) {
            cout << "1. " << currentRoom->left->roomName << endl;
        }

        if (currentRoom->right != nullptr) {
            cout << "2. " << currentRoom->right->roomName << endl;
        }

        cout << "3. Return to Previous Room\n";
    }

    // Move Between Rooms
    void moveToRoom() {
        showRoom();

        int choice;
        cout << "\nEnter your choice: ";
        cin >> choice;

        RoomNode* nextRoom = nullptr;

        if (choice == 1) {
            nextRoom = currentRoom->left;
        }
        else if (choice == 2) {
            nextRoom = currentRoom->right;
        }
        else if (choice == 3) {
            goBack();
            return;
        }
        else {
            cout << "\nInvalid choice!\n";
            return;
        }

        if (nextRoom == nullptr) {
            cout << "\nThis room is not available!\n";
            return;
        }

        roomHistory.push(currentRoom);

        currentRoom = nextRoom;

        visitedRooms.push_back(currentRoom->roomName);

        cout << "\nYou entered " << currentRoom->roomName << "!\n";

        exploreRoom();

        updateMissions();
    }

    // Return to Previous Room using Stack
    void goBack() {
        if (roomHistory.empty()) {
            cout << "\nYou are already at the starting room!\n";
            return;
        }

        currentRoom = roomHistory.top();

        roomHistory.pop();

        cout << "\nReturned to: " << currentRoom->roomName << endl;

        updateMissions();
    }

    // Explore Current Room
    void exploreRoom() {
        int answer;
        string riddleAnswer;

        // Research Lab
        if (currentRoom->roomName == "Research Lab") {
            if (!accessCard) {
                cout << "\nYou found a locked cabinet!\n";
                cout << "Solve this question to get the Access Card.\n";

                cout << "What is 15 + 25? ";
                cin >> answer;

                if (answer == 40) {
                    accessCard = true;

                    inventory.addItem("Access Card");

                    cout << "Correct answer! You found the Access Card.\n";
                }
                else {
                    cout << "Incorrect answer! Try again later.\n";
                }
            }
            else {
                cout << "\nYou have already collected the Access Card.\n";
            }
        }

        // Engineering Room
        else if (currentRoom->roomName == "Engineering Room") {
            if (!powerRestored) {
                cout << "\nThe station power is offline!\n";
                cout << "Solve this question to restore power.\n";

                cout << "What is 12 x 5? ";
                cin >> answer;

                if (answer == 60) {
                    powerRestored = true;

                    inventory.addItem("Power Cell");

                    cout << "Correct answer! Station power restored.\n";
                }
                else {
                    cout << "Incorrect answer! Try again later.\n";
                }
            }
            else {
                cout << "\nThe station power has already been restored.\n";
            }
        }

        // Security Room
        else if (currentRoom->roomName == "Security Room") {
            if (!escapeKey) {
                cout << "\nA security system is asking a riddle!\n";

                cout << "I shine in the night sky. What am I? ";
                cin >> riddleAnswer;

                if (riddleAnswer == "star" ||
                    riddleAnswer == "Star" ||
                    riddleAnswer == "STAR") {

                    escapeKey = true;

                    inventory.addItem("Escape Key");

                    cout << "Correct! You found the Escape Key.\n";
                }
                else {
                    cout << "Wrong answer! Try again later.\n";
                }
            }
            else {
                cout << "\nYou have already collected the Escape Key.\n";
            }
        }

        // Storage Room
        else if (currentRoom->roomName == "Storage Room") {
            if (!storageBonus) {
                cout << "\nYou found some useful supplies!\n";

                score += 10;
                storageBonus = true;

                cout << "You earned 10 bonus points!\n";
            }
            else {
                cout << "\nYou have already collected the supplies here.\n";
            }
        }

        // Medical Room
        else if (currentRoom->roomName == "Medical Room") {
            if (!medicalBonus) {
                cout << "\nYou found a medical supply box!\n";

                score += 10;
                medicalBonus = true;

                cout << "You earned 10 bonus points!\n";
            }
            else {
                cout << "\nYou have already collected the medical supplies.\n";
            }
        }

        // Escape Pod
             
        else if (currentRoom->roomName == "Escape Pod") {
            cout << "\nYou have reached the Escape Pod!\n";

            if (accessCard && powerRestored && escapeKey) {
                escaped = true;

                cout << "\nAll requirements completed!\n";
                cout << "The Escape Pod is ready for launch!\n";
                cout << "\nCONGRATULATIONS, " << playerName << "!\n";
                cout << "You successfully escaped the space station!\n";
            }

            else if (escapeKey && (!accessCard || !powerRestored)) {
                escaped = true;

                cout << "\nYou failed to escape!\n";
                cout << "You reached the Escape Pod without completing all previous missions.\n";
                cout << "GAME OVER!\n";
            }

            else {
                cout << "\nYou cannot escape yet!\n";
                cout << "You still need to complete the following:\n";

                if (!accessCard) {
                    cout << "- Find the Access Card\n";
                }

                if (!powerRestored) {
                    cout << "- Restore the Station Power\n";
                }

                if (!escapeKey) {
                    cout << "- Find the Escape Key\n";
                }
            }
        }


        // Control Center
        else if (currentRoom->roomName == "Control Center") {
            cout << "\nYou are in the main Control Center.\n";
        }
    }

    // Display Visited Rooms
    void showVisitedRooms() {
        cout << "\n------ VISITED ROOMS ------\n";

        if (visitedRooms.empty()) {
            cout << "No rooms visited yet.\n";
            return;
        }

        for (int i = 0; i < visitedRooms.size(); i++) {
            cout << i + 1 << ". " << visitedRooms[i] << endl;
        }
    }

    // Delete Binary Tree
    void deleteTree(RoomNode* node) {
        if (node == nullptr) {
            return;
        }

        deleteTree(node->left);
        deleteTree(node->right);

        delete node;
    }

public:

    // Constructor
    SpaceEscapeGame() {
        root = nullptr;
        currentRoom = nullptr;

        accessCard = false;
        powerRestored = false;
        escapeKey = false;
        escaped = false;

        storageBonus = false;
        medicalBonus = false;

        score = 0;

        createRooms();
        addMissions();
    }

    // Start Game
    void startGame() {
        cout << "====================================\n";
        cout << "       SPACE ESCAPE STATION\n";
        cout << "====================================\n";

        cout << "\nEnter your astronaut name: ";
        getline(cin, playerName);

        currentRoom = root;

        visitedRooms.push_back(root->roomName);

        cout << "\nWelcome, " << playerName << "!\n";
        cout << "You are trapped inside an abandoned space station.\n";
        cout << "Complete your missions and escape safely!\n";

        while (!escaped) {
            int choice;

            cout << "\n========== GAME MENU ==========\n";
            cout << "1. Explore / Move to Another Room\n";
            cout << "2. View Inventory\n";
            cout << "3. View Pending Missions\n";
            cout << "4. Return to Previous Room\n";
            cout << "5. View Visited Rooms\n";
            cout << "6. View Current Location\n";
            cout << "7. Exit Game\n";

            cout << "\nEnter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    moveToRoom();
                    break;

                case 2:
                    inventory.display();
                    break;

                case 3:
                    displayMissions();
                    break;

                case 4:
                    goBack();
                    break;

                case 5:
                    showVisitedRooms();
                    break;

                case 6:
                    cout << "\nCurrent Location: "
                         << currentRoom->roomName << endl;
                    break;

                case 7:
                    cout << "\nYou have exited the game.\n";
                    return;

                default:
                    cout << "\nInvalid choice! Please try again.\n";
            }
        }
        if (accessCard && powerRestored && escapeKey) {

            cout << "\n========== GAME COMPLETED ==========\n";
            cout << "Astronaut: " << playerName << endl;
            cout << "Final Score: " << score << endl;
            cout << "All missions completed!\n";

        } else {

            cout << "\n========== GAME OVER ==========\n";
            cout << "Astronaut: " << playerName << endl;
            cout << "Final Score: " << score << endl;
            cout << "You failed to escape the station!\n";

        }
        }

    // Destructor
    ~SpaceEscapeGame() {
        deleteTree(root);
    }
};

// Main Function
int main() {
    char again;

    do {
        SpaceEscapeGame game;

        game.startGame();

        cout << "\nDo you want to play again? (y/n): ";
        cin >> again;

        cin.ignore(1000, '\n');

    } while (again == 'y' || again == 'Y');

    cout << "\nThank you for playing Space Escape Station!\n";

    return 0;
}
