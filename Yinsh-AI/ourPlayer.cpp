
#include"ourPlayer.h"
#include"ourGame.h"
#include"utils.h"
#include <bits/stdc++.h> //Can we use this??
#include<math.h>

#define pb push_back
#define mp make_pair
#define lli long long int

using namespace std;

struct childSortNode{
  ourGame gameNode;
  lli value;
};

/*General Comments*/
// I think we should use the approach of double rows naming and use the approach of hexagon naming just for input and output, as that is only stored in the game which is used by Yinsh.js
/*GeneralComments over*/


bool compareForMax(const childSortNode &a, const childSortNode &b)
{
    return a.value>b.value;
}

bool compareForMin(const childSortNode &a,const childSortNode &b){
  return a.value<b.value;
}


vector<ourGame> ourPlayer::sortChildren(vector<ourGame> childNodes,bool forMax){
  vector<childSortNode> v;
  for(int i=0;i<childNodes.size();i++){
    lli valueTemp = childNodes[i].computeHeuristicValue();
    childSortNode temp;
    temp.gameNode = childNodes[i];
    temp.value = valueTemp;
    v.push_back(temp);
  }
  if(forMax){
    sort(v.begin(),v.end(),compareForMax);
  }else{
    sort(v.begin(),v.end(),compareForMin);
  }
  vector<ourGame> ans;
  for(int i=0;i<v.size();i++){
    ans.push_back(v[i].gameNode);
  }
  return ans;
}



//Assuming PLayer 0 moves first and PLayer 1 follows

//Constructor
ourPlayer::ourPlayer(int playerNumber,int timeLeft){
  //cin>>player_no>>total_rings>>time_left;
  this->playerNumber =  playerNumber; //1-> Player 1, 2-> Player 2
  this->totalRings =  5; //This version only has to deal with 5 rings
  this->timeLeft =  timeLeft; //will be initialised with full time
  this->myRingsRemoved = 0;//starting with 0 rings
  this->game = new ourGame();
}
//These hexagon and position will be decided by Alpha-Beta pruning
void ourPlayer::placeRing(int playerNo, int x, int y, ourGame* game){
  //Hexagon defines the hexagon in which we will place the new rings
  //position defines the position of that new ring in the hexagon
  // move temp;//it will be automatically initialized by {"",0,0}
  // pair<int,int> p = cartesianToHex(x,y,11);//rows = 11
  // //cout << p.first << " " << p.second << endl;
  // temp.type = "P";
  // temp.hex = p.first; // hexagon
  // temp.pos = p.second; //position
  //cout << "DO I";
  //rings.pb(mp(x,y));
  //cout << " Reach here?" << endl;
  struct boardCell tempboardCell; // Ring is placed, and not marker
  tempboardCell.player = playerNo;//this->playerNumber;
  tempboardCell.containsMarker = false;
  tempboardCell.containsRings = true;
  tempboardCell.canBeUsed = true;
  //cout << "And here?" << endl;
  // cout << (game.board.size()) << endl;
  // cout << game.board[x][y].player << " ";
  // cout << "Definitely I don't reach here, Do I?" << endl;
  game->board[x][y]  = tempboardCell;
  //cout << game.board[x][y].player << endl;
  if(/*this->playerNumber*/playerNo==1){
    game->playerOneRingsOnBoard++;
  }
  else{
    game->playerTwoRingsOnBoard++;
  }
  //return temp;
}

void ourPlayer::moveRing(int playerNo,int xStart, int yStart, int x, int y, ourGame* game){
  //index is the index in rings of the ring which we need to move_end, hexagon and position same  as above
  //We assume that index is never out of bounds and move to be moved is a valid move
  struct move temp;// = {"",0,0};

  // pair<int,int> p = cartesianToHex(x,y,11);//rows = 11
  // temp.type = "M";
  // temp.hex = p.first; // hexagon
  // temp.pos = p.second; //position
  //There will be a marker at original position

  pair<int,int> newMarkerPosition = mp(xStart,yStart);
  if(!(game->board[xStart][yStart]).containsRings){
    // cout << "Galat Chaal chali hai Aapne!" << endl;
    return ;
  }
  //rings[index] = mp(x,y);
  struct boardCell tempBoardCellRing;// {this->playerNumber, true, false}; //Adding ring to the new positions
  tempBoardCellRing.player = playerNo;//this->playerNumber;
  tempBoardCellRing.containsRings = true;
  tempBoardCellRing.containsMarker = false;
  tempBoardCellRing.canBeUsed = true;
  game->board[x][y] = tempBoardCellRing;
  //We can say that this position isn't occupied by any other marker, hence pushBack it
  //markers.pb(newMarkerPosition);
  boardCell tempBoardCellMarker;// = {this->playerNumber,false,true}; // Marker is placed and not ring
  tempBoardCellMarker.player = playerNo;//this->playerNumber;
  tempBoardCellMarker.containsMarker = true;
  tempBoardCellMarker.containsRings = false;
  tempBoardCellMarker.canBeUsed = true;

  game->board[newMarkerPosition.first][newMarkerPosition.second] = tempBoardCellMarker;
  if(/*this->playerNumber*/playerNo==1)
    game->playerOneMarkersOnBoard++;
  else
    game->playerTwoMarkersOnBoard++;
  //return temp;
}

void ourPlayer::removeRing(int playerNo, int x, int y, ourGame* game){
  //Remove the ring, removal of markers will be done separately
  boardCell tempBoard = game->board[x][y];
  //cout << p.first << " " << p.second << " Yolo" <<  endl;
  pair<int,int> rad = cartesianToHex(x,y,11);//rows = 11
  struct move temp;// = {"",0,0};
  temp.type = "X";
  temp.hex = rad.first;
  temp.pos = rad.second;
  //rings.erase(rings.begin()+index);
  boardCell tempBoardCell;// = {this->playerNumber,false,true}; // Marker is placed and not ring
  tempBoardCell.player = 0;
  tempBoardCell.containsMarker = false;
  tempBoardCell.containsRings = false;
  tempBoardCell.canBeUsed = true;
  game->board[x][y] = tempBoardCell;
  if(/*this->playerNumber*/playerNo==1){
    game->playerOneRingsOnBoard--;
  }
  else{
    game->playerTwoRingsOnBoard--;
  }
  //return temp;
}

void ourPlayer::removeRow(int playerNo, int startX, int startY, int endX, int endY, ourGame* game){
  int movementX = endX - startX;
  int movementY = endY - startY;
  // cout << movementX << " " << movementY << endl;
  if(movementX==0){
    //Vertical move
    int mov = movementY/abs(movementY);
    for(int i = min(startY,endY); i<=max(endY,startY); i++){
      boardCell tempBoardCell;
      tempBoardCell.player = 0;
      tempBoardCell.containsMarker = false;
      tempBoardCell.containsRings = false;
      tempBoardCell.canBeUsed = true;
      game->board[startX][i] = tempBoardCell;
    }
  }
  else if(movementY==0){
    //Vertical move
    int mov = movementX/abs(movementX);
    for(int i = min(startX,endX); i<=max(endX,startX); i++){
      boardCell tempBoardCell;
      tempBoardCell.player = 0;
      tempBoardCell.containsMarker = false;
      tempBoardCell.containsRings = false;
      tempBoardCell.canBeUsed = true;
      game->board[i][startY] = tempBoardCell;
    }
  }
  else if(movementX==movementY){
    int mov = movementX/abs(movementX);
    int i,j;
    for(i = min(startX,endX),j=min(endY,startY); i<=max(startX,endX),j<=max(endY,startY); i++,j++){
      boardCell tempBoardCell;
      tempBoardCell.player = 0;
      tempBoardCell.containsMarker = false;
      tempBoardCell.containsRings = false;
      tempBoardCell.canBeUsed = true;
      game->board[i][j] = tempBoardCell;
    }
  }
  else{
    cout << "Galat chal rahe ho, sahi Karo!!!" << endl;
  }
  if(/*this->playerNumber*/playerNo==1){
    game->playerOneMarkersOnBoard = game->playerOneMarkersOnBoard-5;
  }
  else{
    game->playerTwoMarkersOnBoard = game->playerTwoMarkersOnBoard-5;
  }
}

void ourPlayer::moveDecider(int playerNo, string s, ourGame* game){
  int length = s.length();
  //cout << "Length of input: " << length << endl;
  int pointer = 0;
  int rows = (game->rows);
  //cout << rows << " Yeh toh number of rows hai!!" << endl;
  //Here the coordinates are in hex and pos form
  while(pointer<length){
    if(s[pointer]=='R'){
      int startX = s[pointer+3]-'0';
      int startY = s[pointer+5]-'0';
      int endX = s[pointer+10]-'0';
      int endY = s[pointer+12]-'0';
      pair<int,int> convertStart= hexToCartesian(startX, startY, rows);
      pair<int,int> convertEnd = hexToCartesian(endX,endY,rows);
      removeRow(playerNo, convertStart.first, convertStart.second, convertEnd.first, convertEnd.second, game);
      pointer += 14;
    }
    else if(s[pointer]=='P'){
      cout << "Did I come here?" << endl;
      int x = s[pointer+2]-'0';
      int y = s[pointer+4]-'0';
      pair<int,int> coor = hexToCartesian(x,y,rows);
      //cout << coor.first << " " << coor.second << endl;
      placeRing(playerNo,coor.first,coor.second, game);
      pointer += 6;
    }
    else if(s[pointer]=='S'){
      int xStart = s[pointer+2]-'0';
      int yStart = s[pointer+4]-'0';
      int x = s[pointer+8]-'0';
      int y = s[pointer+10]-'0';
      pair<int,int> convertStart= hexToCartesian(xStart, yStart, rows);
      pair<int,int> convertEnd = hexToCartesian(x,y,rows);
      //cout << convertStart.first << " " << convertStart.second << endl;
      //cout << convertEnd.first << " " << convertEnd.second << endl;
      moveRing(playerNo,convertStart.first, convertStart.second, convertEnd.first, convertEnd.second, game);
      pointer += 12;
    }
    else if(s[pointer]=='X'){
      int x = s[pointer+2]-'0';
      int y = s[pointer+4]-'0';
      pair<int,int> coor = hexToCartesian(x,y,rows);
      removeRing(playerNo,coor.first,coor.second, game);
      pointer += 6;
    }
    else{
      cout << "Move is incorrect!!" << endl;
      return;
    }
  }
}

vector<pair<pair<int,int>,pair<int,int> > > ourPlayer::selectAndMoveHelper(int playerNo, int x, int y, int dirX, int dirY, ourGame* game){
  //x,y -> current position of the ring
  //dirX,dirY -> each can be 1,0,-1 denotes kaha check karna hai
  vector<pair<pair<int,int>,pair<int,int> > > ans; //first is the current position of the ring, second is the position in which it can travel
  int i,j;
  pair<int,int> f = mp(x,y);
  int oneMarker=0;
  int rows = game->rows;
  for(i=x+dirX,j=y+dirY;;i=i+dirX,j=j+dirY){
    if(i<0 || j<0 || i>=rows || j>=rows){
      return ans;
    }
    else if(!(game->board[i][j].canBeUsed)){
      //cout << ans.size() << endl;
      // cout << "At this value I came here: " << i << " " << j << endl;
      if(i==0 && j==5){
        cout << "b" << endl;
      }
      return ans;
    }
    else if(game->board[i][j].containsRings){
      return ans;
    }
    else if(game->board[i][j].containsMarker && oneMarker==0){
        oneMarker=1;
        //ans.pb(mp(f,mp(i,j)));
    }
    else if(game->board[i][j].containsMarker && oneMarker==1){
      //Nothing
    }
    else if(game->board[i][j].player==0){
      if(oneMarker==1){
        ans.pb(mp(f,mp(i,j)));
        return ans;
      }
      else{
        //cout << i << " " << j << endl;
        ans.pb(mp(f,mp(i,j)));
      }
    }
    // if(i==0 && j==5){
    //   cout << game->board[i][j].canBeUsed << endl;
    // }
  }
}

vector<pair<pair<int,int>,pair<int,int> > > ourPlayer::selectAndMove(int playerNo,ourGame* game){
  vector<pair<pair<int,int>,pair<int,int> > > ans;
  vector<pair<pair<int,int>,pair<int,int> > > temp;
  // v.insert( v.end(), w.begin(), w.end());
  for(int i=0; i<game->rows; i++){
    for (int j=0; j<game->rows; j++){
      if(game->board[i][j].containsRings){
        if(game->board[i][j].player==playerNo){
          //cout << i << " " << j << endl;
           temp = selectAndMoveHelper(playerNo,i,j,0,1,game);//Total 6 directions
           ans.insert(ans.end(), temp.begin(), temp.end());
           // cout << ans.size() << endl;
           temp = selectAndMoveHelper(playerNo,i,j,0,-1,game);//Total 6 directions
           ans.insert(ans.end(), temp.begin(), temp.end());
           // cout << ans.size() << endl;
           temp = selectAndMoveHelper(playerNo,i,j,1,1,game);//Total 6 directions
           ans.insert(ans.end(), temp.begin(), temp.end());
           // cout << ans.size() << endl;
           temp = selectAndMoveHelper(playerNo,i,j,-1,-1,game);//Total 6 directions
           ans.insert(ans.end(), temp.begin(), temp.end());
           // cout << ans.size() << endl;
           // cout << "KYU " << temp[4].second.first << " " << temp[4].second.second << endl;
           temp = selectAndMoveHelper(playerNo,i,j,1,0,game);//Total 6 directions
           ans.insert(ans.end(), temp.begin(), temp.end());
           // cout << ans.size() << endl;
           temp = selectAndMoveHelper(playerNo,i,j,-1,0,game);//Total 6 directions
           ans.insert(ans.end(), temp.begin(), temp.end());
           // cout << ans.size() << endl;
        }
      }
    }
  }
  return ans;
}

string ourPlayer::markerDeletionHelper(int playerNo, int x, int y, int dirX, int dirY,ourGame* game){
  //vetor<string> ans;
  string temp="";
  int markerCount=0;
  int i,j;
  int rows = game->rows;
  for(i=x,j=y;;i=i+dirX,j=j+dirY){
    if(game->board[i][j].containsMarker && game->board[i][j].canBeUsed){
      if(game->board[i][j].player==playerNo){
        markerCount++;
      }
      if(markerCount==5){
        pair<int,int> initial = cartesianToHex(x,y,rows);
        pair<int,int> finall = cartesianToHex(i,j,rows);
        string result;
        ostringstream convert;
        convert << initial.first;
        result = convert.str();

        string result1;
        ostringstream convert1;
        convert1 << initial.second;
        result1 = convert1.str();

        string result2;
        ostringstream convert2;
        convert2 << finall.first;
        result2 = convert2.str();

        string result3;
        ostringstream convert3;
        convert3 << finall.second;
        result3 = convert3.str();

        temp = "RS " + result + " " + result1 + " RE " + result2 + " " + result3;
        //ans.pb(temp);
        break;
      }
    }
    else break;
  }
  return temp;
}

vector<string> ourPlayer::markerDeletion(int playerNo, ourGame* game){
  int rows = game->rows;
  vector<string> ans;
  string temp = "";
  for(int i=0; i<rows; i++){
    for(int j=0; j<rows; j++){
      if(game->board[i][j].containsMarker && game->board[i][j].canBeUsed && game->board[i][j].player ==playerNo){
        temp = markerDeletionHelper(playerNo,i,j,0,1,game);//Total 6 directions
        if(temp.length()!=0){
          ans.pb(temp);
        }

        temp = markerDeletionHelper(playerNo,i,j,0,-1,game);//Total 6 directions
        if(temp.length()!=0){
          ans.pb(temp);
        }

        temp = markerDeletionHelper(playerNo,i,j,1,1,game);//Total 6 directions
        if(temp.length()!=0){
          ans.pb(temp);
        }

        temp = markerDeletionHelper(playerNo,i,j,-1,-1,game);//Total 6 directions
        if(temp.length()!=0){
          ans.pb(temp);
        }

        temp = markerDeletionHelper(playerNo,i,j,1,0,game);//Total 6 directions
        if(temp.length()!=0){
          ans.pb(temp);
        }

        temp = markerDeletionHelper(playerNo,i,j,-1,0,game);//Total 6 directions
        if(temp.length()!=0){
          ans.pb(temp);
        }
      }
    }
  }
  return ans;
}

vector<string> ourPlayer::selectAndMoveFinal(int playerNo, ourGame* game){
  vector<string> ans;
  vector<pair<pair<int,int>, pair<int,int> > > list = selectAndMove(playerNo, game);
  string temp="";
  int rows = game->rows;
  //cout << list.size() << endl;
  for(int i=0; i<list.size(); i++){
    int s1 = list[i].first.first;
    int s2 = list[i].first.second;
    int m1 = list[i].second.first;
    int m2 = list[i].second.second;
    pair<int,int> initial = cartesianToHex(s1,s2,rows);
    pair<int,int> finall = cartesianToHex(m1,m2,rows);

    string result;
    ostringstream convert;
    convert << initial.first;
    result = convert.str();

    string result1;
    ostringstream convert1;
    convert1 << initial.second;
    result1 = convert1.str();

    string result2;
    ostringstream convert2;
    convert2 << finall.first;
    result2 = convert2.str();

    string result3;
    ostringstream convert3;
    convert3 << finall.second;
    result3 = convert3.str();

    temp = "S " + result + " " + result1 + " M " + result2 + " " + result3;
    ans.pb(temp);
  }
  return ans;
}

vector<string> ourPlayer::removeRingFinal(int playerNo, ourGame* game){
  int rows = game->rows;
  vector<string> ans;
  string temp;
  for(int i=0; i<rows; i++){
    for(int j=0; j<rows; j++){
      if(game->board[i][j].player==playerNo && game->board[i][j].containsRings){
        pair<int,int> initial = cartesianToHex(i,j,rows);
        string result;
        ostringstream convert;
        convert << initial.first;
        result = convert.str();

        string result1;
        ostringstream convert1;
        convert1 << initial.second;
        result1 = convert1.str();

        temp = "X " + result + " " + result1;
        ans.pb(temp);
      }
    }
  }
  return ans;
}

vector<string> ourPlayer::removeMarkerAndRing(int playerNo, ourGame* game){
  //This function checks if we can remove the rings or not and returns a vector of strings
  vector<string> ans;
  vector<string> first = markerDeletion(playerNo, game);
  string temp;
  for(int i=0; i<first.size(); i++){
    string removeMarkerOne = first[i];
    ourGame* firstGame = new ourGame();
    firstGame->copyTheBoard(game);
    moveDecider(playerNo,removeMarkerOne,firstGame);
    vector<string> firstRing = removeRingFinal(playerNo, game);
    for(int j=0; j<firstRing.size(); j++){
      string t = removeMarkerOne + " " + firstRing[j];
      ans.pb(t);
    }
  }
  return ans;
}

vector<string> ourPlayer::allDeletions(int playerNo, ourGame* game){
    vector<string> ans;
    vector<string> firstDeletion = removeMarkerAndRing(playerNo,game);
    // string move="";
    if(firstDeletion.size()==0)
      return ans;
    else{
      for(int i=0; i<firstDeletion.size(); i++){
        string firstMove = firstDeletion[i];
        // move = firstMove;
        ourGame* firstGame = new ourGame();
        firstGame->copyTheBoard(game);
        moveDecider(playerNo,firstMove,firstGame);
        if((playerNo==1 && firstGame->playerOneRingsOnBoard==2)||(playerNo==2 && firstGame->playerTwoRingsOnBoard==2)){
          ans.pb(firstMove);
        }
        else{
          vector<string> secondDeletion = removeMarkerAndRing(playerNo,firstGame);
          if(secondDeletion.size()==0){
            ans.pb(firstMove);
          }
          else{
            for(int j=0; j<secondDeletion.size(); j++){
              string secondMove = secondDeletion[j];
              ourGame* secondGame = new ourGame();
              secondGame->copyTheBoard(firstGame);
              moveDecider(playerNo,secondMove,secondGame);
              if((playerNo==1 && secondGame->playerOneRingsOnBoard==2)||(playerNo==2 && secondGame->playerTwoRingsOnBoard==2)){
                string temp = firstMove + " " + secondMove;
                ans.pb(temp);
              }
              else{
                vector<string> thirdDeletion = removeMarkerAndRing(playerNo, firstGame);
                if(thirdDeletion.size()==0){
                  string temp = firstMove + " " + secondMove;
                  ans.pb(temp);
                }
                else{
                  for(int k=0; k<thirdDeletion.size(); k++){
                    string thirdMove = thirdDeletion[k];
                    ourGame* thirdGame = new ourGame();
                    thirdGame-> copyTheBoard(secondGame);
                    moveDecider(playerNo,thirdMove,thirdGame);
                    string tempp = firstMove+ " " + secondMove + " " + thirdMove;
                    ans.pb(tempp);
                  }
                }
              }
            }
          }
        }
      }
    }
    return ans;
}

vector<string> ourPlayer::moveList(int playerNo, ourGame* game){
  //Returns a list of moves
  vector<string> ans;
  vector<string> firstRound = allDeletions(playerNo, game);
  //cout << "Do I Reach here?" << endl;
  if(firstRound.size()==0){
    //Nothing to delete
    vector<string> fr = selectAndMoveFinal(playerNo, game);
    if(fr.size()==0){
      cout << "NO MOVE LEFT" << endl;
    }
    else{
      for(int i=0; i<fr.size(); i++){
        ourGame* afterFirstMove = new ourGame();
        afterFirstMove->copyTheBoard(game);
        string firstMove = fr[i];
        moveDecider(playerNo, firstMove, afterFirstMove);
        vector<string> dr = allDeletions(playerNo, afterFirstMove);
        if(dr.size()==0){
          //Nothing to delete
          ans.pb(firstMove);
        }
        else{
          for(int j=0; j<dr.size(); j++){
            string temp = firstMove + " " + dr[i];
            ans.pb(temp);
          }
        }
      }
    }
  }
  else{
    //There are deletions possible
    // cout << "First Round Size: " << firstRound.size() << endl;
    for(int i=0; i<firstRound.size(); i++){
      // cout << "Count" << endl;
      ourGame* afterFirstMove = new ourGame();
      afterFirstMove->copyTheBoard(game);
      string fm = firstRound[i];
      moveDecider(playerNo, fm, afterFirstMove);
      vector<string> sr = selectAndMoveFinal(playerNo,afterFirstMove);
      if(sr.size()==0){
        //No move to play ring
        ans.pb(fm);
      }
      else{
        // cout << sr.size() << endl;
        // cout << sr[18] << endl;
        // cout << fm << endl;
        for(int j=0; j<sr.size(); j++){
          ourGame*  afterSecondMove = new ourGame();
          afterSecondMove->copyTheBoard(afterFirstMove);
          string sm = sr[j];
          moveDecider(playerNo, sm, afterSecondMove);
          vector<string> tr = allDeletions(playerNo, afterSecondMove);
          //cout << tr.size() << endl;
          // cout << sm << " " << j << "\n";
          if(tr.size()==0){
            //Nothing to delete
            string temp = fm + " " + sm;
            //cout << temp << endl;
            ans.pb(temp);
            // if(j==18)
            //   cout << "What happens?" << endl;
          }
          else{
            //Something to delete
            for(int k=0; k<tr.size(); k++){
              string temp = fm + " " + sm + " " + tr[k];
              ans.pb(temp);
            }
          }
        }
      }
    }
  }
  return ans;
}

/*
*The iterative deepening version of minimax with
*some max time and depth alloted
*/
lli ourPlayer::idMinimax(ourGame gameNode,int max_depth,double maxTime){
  int depth = 0;
  double tempTime=0;
  struct timespec start_time,move_time;
  htMap.clear();

  //start noting time
  clock_gettime(CLOCK_REALTIME, &start_time);
  lli bestScore=-INFINITY;
  for(depth=0;depth<=max_depth;depth++){
    lli value = minimax(gameNode,depth,true,-INFINITY,INFINITY);
    bestScore = max(bestScore,value);
    //compute time to solve for depth
    clock_gettime(CLOCK_REALTIME, &move_time);
    double seconds = (double)((move_time.tv_sec+move_time.tv_nsec*1e-9) - (double)(start_time.tv_sec+start_time.tv_nsec*1e-9));
    //return value if time exceeded
    if(seconds>=maxTime){
      htMap.clear();
      return value;
    }
  }
  htMap.clear();
  return bestScore;
}


//initialize with alpha = -INFINITY & beta = INFINITY
long long int ourPlayer::minimax(ourGame gameNode,int depth,bool isMax,lli alpha,lli beta){


  if(depth==0){
    return gameNode.computeHeuristicValue();
  }

  lli bestScore;

  //if our player's turn
  if(isMax){
    bestScore=-INFINITY;
    vector<ourGame> childVector;
    childVector = gameNode.children();//assuming children function returns an vector of possible gameNodes
    childVector = sortChildren(childVector,true);
    for(int i=0;i<childVector.size();i++){
      lli value = minimax(childVector[i],depth+1,false,alpha,beta);
      alpha = max(alpha,value);
      bestScore = max(value,bestScore);
      if(alpha>=beta){
        return value;
      }
    }
    return bestScore;
  }

  //if opponent's turn
  if(!isMax){
    bestScore=INFINITY;
    vector<ourGame> childVector;
    childVector = gameNode.children();//assuming children function returns an vector of possible gameNodes
    childVector = sortChildren(childVector,false);
    for(int i=0;i<childVector.size();i++){
      lli value = minimax(childVector[i],depth+1,true,alpha,beta);
      beta = min(beta,value);
      bestScore = min(value,bestScore);
      if(alpha>=beta){
        return value;
      }
    }
    return bestScore;
  }

}

/*
void ourPlayer::play(){
  string opponentMove;
  if(this->playerNumber==1){
    move m = this->game->getMove(this->playerNumber);
    this->game->execute_move(m);
    cout<<toString(m);
    //cin>>opponentMove;
  }
  cin>>opponentMove;
  while(!this->game->ended()){
    this->game->execute_move(toMove(opponentMove));
    move m = this->game->getMove(this->playerNumber);
    this->game->execute_move(m);
    cout<<toString(m);
    cin>>opponentMove;
  }
}
*/
/*
vector<string> ans;
vector<string> beforeDeletion = markerDeletion(playerNo, game);
//string deletionString="";
for(int i=0; i<beforeDeletion.size(); i++){
  string temp = beforeDeletion[i];
  ourGame* tempGame = new ourGame();
  tempGame->copyTheBoard(game);
  moveDecider(playerNo,temp,tempGame);
  //deletionString = deletionString + temp + " ";
  //vector<pair<pair<int,int>,pair<int,int> > > tt = selectAndMove(playerNo, game);
  vector<string> selectAndMoveM = selectAndMoveFinal(playerNo,tempGame);
  for(int j=0; j<selectAndMoveM.size(); j++){
    string tempTemp = selectAndMoveM[j];
    ourGame* tempTempGame = new ourGame();
    tempTempGame->copyTheBoard(tempGame);
    moveDecider(playerNo,tempTemp,tempTempGame);

    vector<string> markerDelete = markerDeletion(playerNo,tempTempGame);
    if(markerDelete.size()==0){
      string ansMember = temp + " " + tempTemp;
      ans.pb(ansMember);
    }
    else{
      for(int k=0; k<markerDelete.size(); k++){
        string tempTemp_Temp = markerDelete[k];
        // ourGame tempTemp_TempGame = new ourGame();
        // *(tempTemp_TempGame) = *(tempTempGame);
        // moveDecider(playerNo,tempTemp_Temp, tempTemp_TempGame);
        string ansMember = temp + " " + tempTemp + " " + tempTemp_Temp;
        ans.pb(ansMember);
      }
    }
  }
}
//vector<pair<pair<int,int>,pair<int,int> > > tt = selectAndMove(playerNo,game);
vector<string> snm = selectAndMoveFinal(playerNo, game);
for(int i=0; i<snm.size(); i++){
  string s = snm[i];
  ourGame* tempGame = new ourGame();
  tempGame->copyTheBoard(game);
  moveDecider(playerNo,s,tempGame);
  vector<string> snms = markerDeletion(playerNo, tempGame);
  if(snms.size()==0){
    ans.pb(s);
  }
  else{
    for(int j=0; j<snms.size(); j++){
      string ss = snms[j];
      string ansMember = s + " " + ss;
      ans.pb(ansMember);
    }
  }
}
return ans;
//All the markers deleted
*/
