/*
	FileName: MasterMind.cpp
	Author: InCheat
	Date: 2018 / 10 / 09
	Version: 1
	Description:
		Simple MasterMind Game Code with clean code principle. (For Practice)
		
	TODO:
		Reconstruct
		
	UNDO: 
		UserInput Check
		Exception Address
	
	
*/
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;
 
class Game{
	public:
		void run(){
			showWelcome();
			selectLevel();
			start();
			score();
			end();
		}
		
		void set_welcomeMsg(string welcomeMsg){
			_welcomeMsg = welcomeMsg;
		}
		string get_welcomeMsg(){
			return _welcomeMsg;
		}
	
		void set_level(int level){
			_level = level;
		}
		int get_level(){
			return _level;
		}
	
		void set_score(int score){
			_score = score;
		}
	
		int get_score(){
			return _score;
		}
	
	
		virtual void showWelcome() = 0;
		virtual void select_level() = 0;
		virtual void start() = 0;
		virtual void score() = 0;
		virtual void end() = 0;

	private:
		string _welcomeMsg;
		int _level;	
		int _score;
};

class MasterMind : public Game {
	public:
		MasterMind() {
			string mastermind_welcomeMsg = 
			"============================================\n"
			"==     Welcome to play Master Mind ! !    ==\n"
			"==                                        ==\n"
			"==   press ' S / s ' to start new game    ==\n"
			"==  press  ' Esc ' to exit this program   ==\n"
			"============================================\n";
			
			this->set_welcomeMsg(mastermind_welcomeMsg);
			this->set_level(0);
			this->A_num = 0;
			this->B_num = 0;
		}
		MasterMind(int level, string& welcomeMsg){
			this->set_level(level);
			this->set_welcomeMsg(welcomeMsg);
		}
		
		
	private:
		void showWelcome() {
			cout << this->get_welcomeMsg();
			cout << "Game is Running!!";
			cout << endl;
		}
			
		void selectLevel() {
			// TODO:: Need to check user input is vaild or not
			// 
			int _level;
			cout << "Please choose Game Level:\n";
			cin >> _level;
			this->set_level(_level);
			cout << "Your Level is " << this->get_level();
			cout << endl;
		}
		
		void start() {
			genAnswer(this->get_level());
			do{
				init();
				userGuess();
				answerCheck(this->_answer, this->_guess);
				showHint();
			}while(!gameOver());
		}
		
		void genAnswer(int digits){
			int* answerArr = new int[digits];
			int* candidateAnswerArr = genCandidateAnswerArr();
			
			int shuffleTimes = this->_randomSeed;
			arrShuffle(candidateAnswerArr, shuffleTimes);
			answerSelect(answerArr, candidateAnswerArr, digits);
			
			this->_answer = answerArr;
			this->_lengthAnswer = digits;
		}
		
		int* genCandidateAnswerArr(){
			int* tmpArr = new int[10];
			for(int i = 0; i < 10; i ++) {
				tmpArr[i] = i;
			}
			return tmpArr;
		}
		
		void arrShuffle(int* arr, int times){
			srand(time(NULL));
			for(int i = 0; i < times; i++){
				int Base_IndexforChanged = i%10;
				int SelectInRandom_IndexToChanged = rand()%10;
				swap(arr, Base_IndexforChanged, SelectInRandom_IndexToChanged);
			}
		}
		
		void swap(int* arr, int index_1, int index_2){
			int tmp = arr[index_1];
			arr[index_1] = arr[index_2];
			arr[index_2] = tmp;
		}
		
		void answerSelect(int* answerArr, int* candidateAnswerArr, int NumOfSelected){
			for(int i = 0; i < NumOfSelected; i ++){
				answerArr[i] = candidateAnswerArr[i];
			}
		}
		
		void init(){
			clearHint();
		}
		
		void clearHint(){
			this->A_num = 0;
			this->B_num = 0;
		}
		
		void userGuess() {
			// TODO:: Need to check user input is vaild or not
			string userInput;
			cout << "Please guess your numbers ";
			cout << "( " << this->get_level() << "digits ): ";
			cin >> userInput;
			userInputProcessing(userInput); 
			this->_guessNum ++;
		}
		
		void userInputProcessing(string userInput){
			// if userInput is not vaild, Error occur!!
			const char* userInputBuffer = userInput.c_str();
			int* guessBuffer = new int[this->_lengthAnswer];
			for(int i = 0; i < this->_lengthAnswer; i ++){
				guessBuffer[i] = charToInt(userInputBuffer[i]);
			}
			this->_guess = guessBuffer;
		}
		
		int charToInt(char item){
			int item_ASCII = (int)item;
			if (item_ASCII > int('9')) return 0; // Input check (not finished) 
			
			int offset_ASCII_char_int = int('0');
			return item_ASCII - offset_ASCII_char_int;
		}
		
		void answerCheck(int* answer, int* guess) {
			int GuessNumPosition = 0;
			int AnswerNumPosition = 0;
			int* buckets = genBuckets();
			for(int i = 0; i < this->_lengthAnswer; i ++){
				int bucketNoOfGuessNum = guess[i];
				buckets[bucketNoOfGuessNum] = GuessNumPosition++;
			}
			for(int i = 0; i < this->_lengthAnswer; i++ ){
				int bucketNoOfAnswerNum = answer[i];
				if (buckets[bucketNoOfAnswerNum] == AnswerNumPosition++){
					this->A_num++;
				}else if(buckets[bucketNoOfAnswerNum] != (-1)){
					this->B_num++;
				}
			}
		}
		
		int* genBuckets(){
			int* buckets = new int[10];
			for(int i = 0; i < 10; i++){
				buckets[i] = -1;
			} 
			return buckets;
		}
		
		void showHint(){
			cout << "You Got ";
			cout << this->A_num << "A";
			cout << this->B_num << "B";
			cout << endl;
		}
		
		bool gameOver() {
			return (this->A_num == this->_lengthAnswer) ? true : false;
		}
		
		void score() {
			
			this->set_score((100 - this->_guessNum));
		}
		
		void end() {
			showScore();
			cout << "Thanks for your playing! bye bye!!" << endl;
		}
		
		void showScore(){
			cout << "You got " << this->get_score() << "points!!";
			cout << endl;
		}
		
		void showAnswer(){
			cout << "The Answer is ";
			showArray(this->_answer, this->_lengthAnswer);
		}
		
		void showArray(int *arr, int length){
			for(int i = 0; i < length; i ++){
				cout << arr[i];
			}
			cout << endl;
		}
		
	private:
		int* _answer;
		int* _guess;
		int _lengthAnswer; 
		int _guessNum;
		int A_num;
		int B_num;
		
		static const int _randomSeed = 100;
		
};


int main() {
	Game* mastermind = new MasterMind();
	mastermind->run();
	
	return 0;
	
}
