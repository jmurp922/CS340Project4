#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;
class Process {
    
    private :
    //Where in memory the processses start and end
    int processStart;
    int processEnd;
    //Total bytes of the process
    int totalBytes;
    //Name of the process
    string processName;
    Process* processArray;
    
    
    public :
    Process() {
        //default constructor
    }
    
    Process(int start, int bytes, string name, int end) {
        processStart = start;
        totalBytes = bytes;
        processName = name;
        processEnd = end;
    }
    void setStart(int start){
        processStart = start;
    }
    void setEnd(int end) {
        processEnd = end;
    }
    
    string getName() {
        return processName;
    }
    
    int getStart() {
        return processStart;
    }
    
    int getEnd() {
        return processEnd;
    }
    
    int getBytes() {
        return totalBytes;
    }
    
    void stat() {
        //Stat print out function.
        cout << "[" << this -> getStart() << " : " << this -> getEnd() << "] Process " << this -> getName() << endl;
    }
    
};

int main(int argc, const char * argv[]) {
    // Use the s to i function to convert argv[1] which is the location of command line argument of the total bytes used into an integer.
    int globalMax = 0;
    int totalBytes = stoi(argv[1]) - 1;
    int start = 0;
    bool arr[totalBytes];
    bool complete = false;
    // If the removed bool == false then that means memory is contigious and we dont have to search for an open spot
    bool removed = false;
    vector<Process> vec;
    for (int i = 0; i < totalBytes; i++) {
        arr[i] = false;
    }
    //Declare a hashmap to keep track of the processes currently being stored. The key is the string, or process name and the value is the actual proces object itself.
    while (!complete) {
        cout << "\nEnter a command. \n";
        string s;
        string cmdrq = "rq", cmdrq2 = "RQ";
        string cmdstat = "STAT", cmdstat2 = "stat";
        string cmdrl = "RL";
        string cmdc = "C";
        getline(cin, s);
        istringstream stringStream(s);
        //Using std::string::npos to determine when the end of the string is met and comparing it to string.find
        if (s.find(cmdrq) != std::string::npos || s.find(cmdrq2) != std::string::npos) {
            //cout << "Entered the RQ block" <<endl;
            string temp = s;
            //This substring is indices 0 -> first white space
            string cmd = temp.substr(0, temp.find(" "));
            //Set the string to the substring after the first space encountered + 1 to cut out the RQ command entered
            temp = temp.substr(temp.find(" ") + 1);
            //The process name is entered after the first space, take that and assign it as pname.
            string pname = temp.substr(0, temp.find(" "));
            temp = temp.substr(temp.find(" ") + 1);
            //Parse the process name, now parse the bytes
            string bytes = temp.substr(0, temp.find(" "));
           // cout << "PROCESS NAME IS " << pname << " AND HAS " << bytes << " BYTES\n";
            //Convert the bytes from a string into an integer so we can store it into the process object
            int numBytes;
            stringstream strBytes(bytes);
            strBytes >> numBytes;
            Process a(start, numBytes, pname, start + numBytes - 1);
            if (!removed && totalBytes > start) {
            //Create the process object using our parsed variables
                if (a.getStart() == 0) {
                    //If start is 0 put process in at index 0, subtract one from the end since its 0 -> end - 1 since 0 is the starting position
                    //a.setEnd(a.getEnd() - 1);
                }
                else if (!removed && a.getStart() == vec[vec.size() - 1].getEnd()) {
                    //Since the last process entered has to be the last in the queue due to nothing being removed from the queue yet in this block, vec[vec.size()-1] will return the last process entered to the queue. Since the 0 index messes up the numbers we need to compensate and fix for the start being the same as the end memory index, so we add + 1 if they're equal to guarantee memory.
                    a.setStart(a.getStart() + 1);
                }
                vec.push_back(a);
                int j = a.getBytes() + start;
                if (a.getStart() != 0) {
                    j += 1;
                }
                for (int i = start; i < j; i++) {
                    arr[i] = true;
                    //cout << "INDEX " << i << " MARKED" << endl;
                }
                start += a.getBytes();

            }
            if (removed) {
                bool ins = false;
                //Trying to find the best fit gap
                int tempMin = 0, tempMax = 0, tempDiff = 0;
                int bestMin = 0, bestMax = 0, bestDiff = 0;
                for (int i = 0; i < vec.size(); i++) {
                    if (globalMax < vec[i].getEnd()) {
                        globalMax = vec[i].getEnd();
                    }
                    if (tempMax != vec[i + 1].getStart()) {
                        //This if statement is created if there isn't an vec[i+1] so it doesnt just default to the old value already stored.
                        tempMax = vec[i + 1].getStart();
                        tempMin = vec[i].getEnd();
                        tempDiff = tempMax - tempMin;
                    }
                    
                    if ((bestMin == 0 && bestMax == 0) || (bestDiff > tempDiff)) {
                        //If the bestMin and bestMax valuse havent been initialized, set them equal to the first vector in the for loop. If it is initialized and the bestDifference is bigger than the tempDifference which has been verified to already fit the new process, set that to the best fit.
                        bestMin = tempMin;
                        bestMax = tempMax;
                        bestDiff = tempDiff;
                    }
                    if (totalBytes - globalMax < bestDiff && totalBytes - globalMax > a.getBytes()) {
                            //If the global max from all the vectors is smaller than the best small fit difference and the totalbytes - global max is greater than a's bytes, swap it. If it was = you can just straight up insert it.
                        if (globalMax + a.getBytes() < totalBytes && totalBytes - globalMax > a.getBytes()){
                            bestMin = globalMax;
                            bestMax = bestMin + a.getBytes();
                            bestDiff = bestMax - bestMin;
                        }
                        }
                    else {
                            bestMin = tempMin;
                            bestMax = tempMax;
                            bestDiff = tempDiff;
                        }

                    if (tempDiff == a.getBytes()) {
                        //Perfect fit insert it there.
                        a.setEnd(tempMax + a.getBytes());
                        a.setStart(tempMax + 1);
                        vec.push_back(a);
                        for (int i = tempMax + 1; i < totalBytes; i++) {
                            arr[i] = true;
                        }
                        //Need to put a break otherwise this turns into an infinite loop
                        break;
                    }
                }
                //Check the tempDifference variable. If there was no best fit, insert it at the end of the process Vector
                if (bestDiff == 0 < a.getBytes() && ins == false) {
                    for (int i = globalMax + 1; i < a.getBytes() + globalMax + 1; i ++) {
                        arr[i] = true;
                    }
                    a.setStart(globalMax + 1);
                    a.setEnd(a.getStart() + a.getBytes() - 1);
                    vec.push_back(a);
                }
                else {
                    //There was a good fit, insert it.
                    if (bestMin + 1 + a.getBytes() > totalBytes) {
                        //Initalize variables and check to see if theres an entry exceeding the total bytes, if there is we need to find a new location for the entry and insert it there
                        int fixMin = 0, fixMax = 0, fixDiff = 0;
                        fixMin = vec[0].getEnd();
                        fixMax = vec[1].getStart();
                        fixDiff = fixMax - fixMin;
                        for (int i = 0; i < vec.size(); i++) {
                            if (vec[i+1].getStart() - vec[i].getEnd() > a.getBytes() && vec[i+1].getStart() - vec[i].getEnd() < fixDiff) {
                                fixMax = vec[i + 1].getStart();
                                fixMin = vec[i].getEnd();
                                fixDiff = fixMax - fixMin;
                            }
                        }
                        bestMin = fixMin;
                    }
                    for (int i = bestMin + 1; i < bestMin + 1 + a.getBytes(); i++) {
                        arr[i] = true;
                    }
                    a.setStart(bestMin + 1);
                    a.setEnd(a.getStart() + a.getBytes() - 1);
                    vec.push_back(a);
                    globalMax = a.getEnd();
                }
            }
        }
        else if (s.find(cmdstat) != std::string::npos) {
            // STAT TEST
            bool locked = false;
            int startChain = 0, endChain = 0;
            vector<Process> temp;
            for (int i = 0; i < totalBytes; i++) {
                // IF the current index of the array is true, search through our vector to find the index of the current vector occupying that space in memory, print out its stat function and then set i equal to its end. That way it gets incremented and we can start looking into the next segment of memory
                if (arr[i] == true) {
                    for (int j = 0; j < vec.size(); j++) {
                        if (vec[j].getStart() == i) {
                            //cout << "Found true entry at index : " << i << endl << "Current memory indices is from " << vec[j].getStart() << " to " << vec[j].getEnd() << endl;
                            vec[j].stat();
                            i = vec[j].getEnd();
                        }
                    }
                } // If arr i == true block
                else if (arr[i] == false) {
                    //CHAIN BEGINS. We have to chain through free memory to find all the open memory locations.
                    if (!locked) {
                        //If it's not locked, this is where the chain starts.
                        startChain = i;
                        locked = true;
 
                    }
                    if ((locked && arr[i + 1]) || i == totalBytes - 1) {
                        //If it's locked and the next entry is true OR we've reached the end of the total memory the program is alloted, (totalBytes - 1) : set the chain end and print out the free memory
                        endChain = i + 1;
                        if (i != totalBytes - 1) {
                            //If it's not the end print out the memory
                            cout << "[" << startChain << " : " << endChain << "] Free" << endl;
                            locked = false;
                        }
                        else {
                            //If it's the end print out END
                            cout << "[" << startChain << " : " << "END] Free" << endl;
                        }
                    }
                    
                }
            }
        }
        
        else if (s.find(cmdrl) != std::string::npos) {
            //Now when we're inserting a process we can't go from end to end since there are now memory gaps, now we have to manually check the bits
            removed = true;
            string temp = s;
            //This substring is indices 0 -> first white space
            string cmd = temp.substr(0, temp.find(" "));
            //Set the string to the substring after the first space encountered + 1 to cut out the RQ command entered
            temp = temp.substr(temp.find(" ") + 1);
            //The process name is entered after the first space, take that and assign it as pname.
            string pname = temp.substr(0, temp.find(" "));
            for (int i = 0; i < vec.size(); i++) {
                if (vec[i].getName().find(pname) != std::string::npos) {
                    //Find the matching process and deallocate the memory stored in our boolean array, then delete the entry in our vector.
                    for (int j = vec[i].getStart(); j != vec[i].getEnd(); j++) {
                        arr[j] = false;
                    }
                    vec.erase(vec.begin() + i);
                }
            }

            
        }
        else if (s.find(cmdc) != std::string::npos) {
            cout << "Entered the C block" <<endl;
            if (!removed) {
                //If no processes were removed, then you dont need to compact it any more. Everything is placed in order with no gaps.
                cout << "Processes are already compact." << endl;
            }
            else {
                //Take care of the gaps........
                int startTemp = 0;
                bool tempArr[totalBytes];
                //Initalize the temp array to false.
                for (int i = 0; i < totalBytes; i++) {
                    tempArr[i] = false;
                }
                for (int i = 0; i < vec.size(); i++) {
                    if (startTemp != 0 ) {
                        //Set the start to startTemp + 1 if start isn't 0 since it's memory location is         0 .... n - 1
                        vec[i].setStart(startTemp + 1);
                    }
                    else {
                        // If it's the start just  set the start to tartTemp
                        vec[i].setStart(startTemp);
                    }
                    if(vec[i].getStart() == 0 ) {
                        startTemp += vec[i].getBytes() - 1;
                    }
                    else {
                        startTemp += vec[i].getBytes();
                    }
                    vec[i].setEnd(startTemp);
                }
                
                for (int i = 0; i < vec.size(); i++) {
                    for (int j = vec[i].getStart(); j < vec[i].getEnd(); j++) {
                        tempArr[j] = true;
                    }
                }
                
                //Since everythings compact we don't need to check for gaps now can just add new processes to the back of the queue
                removed = false;
                //Can't set an array equal to another array, so have to use the memcpy function.
                memcpy(arr, tempArr, totalBytes);
            }
        }
        else {
            cout << "Invalid command, exiting. \n";
            complete = true;
        }
    }
    return 0;
}
