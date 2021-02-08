#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <string>
#include <chrono>
#include <cctype>
#include <utility>
#include <unordered_map>
#include <iterator>
#include <sstream>
#include <iomanip>

using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::vector;
using std::list;
using std::fstream;
using std::ios;
using std::string;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::list;
using std::unordered_map;
using std::pair;
using std::stringstream;
using std::fixed;
using std::setprecision;

class DirectedGraph {
private:
  struct edge {
	char e = '\0';
	string adjacent = "";
  };

  // vertex with incomingEdges.size() == 0 is the starting vertex of the graph
  struct vertex {
	string word;
	vector<edge> incomingEdges;
	vector<edge> outgoingEdges;
	vector<edge> edgesViaPeriods;
  };

  string inputFile = "", outputFile = "", expectedOutputFile = "";

  bool pastFirstWord = false;
  char edgeChar = '\0', endChar = '\0';
  int startingWordIndex = 0;
  string startingWord = "", tmpString = "", prevWord = "", currWord = "";

  unordered_map<string, vertex> graph;
  vector<string> inputWords;


  void readInputData(fstream& inputFilePointer) {
	stringstream ss;
	tmpString = "";	

	cout << "Reading input data ....." << endl;

	inputFilePointer.seekg( 0 );
	ss << inputFilePointer.rdbuf();

	while ( ss >> tmpString ) {
	  inputWords.push_back( tmpString );
	}
	tmpString = "";	

	cout << "Data read-in complete!" << endl << endl;
  }

  void findStartingWord() {
	tmpString = "";

	for ( int i = 0; i < inputWords.size(); i++ ) {
	  tmpString = inputWords[i];

	  if ( tmpString[tmpString.size() - 1] == '.' )
		continue;
	  else {
		if ( tmpString[tmpString.size() - 1] == ',' ) {
		  edgeChar = ',';
		  startingWord = tmpString.substr( 0, tmpString.size() - 1 );
		  startingWordIndex = i;
		  prevWord = startingWord;
		  //cout << "starting word: " << startingWord << endl;
		  break;
		}		
		else {
		  edgeChar = '\0';
		  startingWord = tmpString;
		  startingWordIndex = i;
		  prevWord = startingWord;
		  //cout << "starting word: " << startingWord << endl;
		  break;
		}
	  }
	}
	tmpString = "";

  }

  void addOutgoingEdge( string from, string to, char e ) {
	edge tmpEdge;
	tmpEdge.e = e;
	tmpEdge.adjacent = to;
	bool alreadyExists = false;

	// if the to vertex already exists, update its incoming edges
	// if the to vertex does not exist already, make one and update its incoming edges
	if ( graph.find( from ) != graph.end() ) {
	  if ( e == '.' ) {
		for ( int j = 0; j < graph[from].edgesViaPeriods.size(); j++ ) {
		  if ( graph[from].edgesViaPeriods[j].adjacent == tmpEdge.adjacent ) {
			alreadyExists = true;
			break;
		  }
		}
	  }
	  else {
		for ( int j = 0; j < graph[from].outgoingEdges.size(); j++ ) {
		  if ( graph[from].outgoingEdges[j].adjacent == tmpEdge.adjacent ) {
			alreadyExists = true;
			break;
		  }
		}		
	  }	

	  if ( !alreadyExists ) {
		if ( e == '.' )
		  graph[from].edgesViaPeriods.push_back( tmpEdge );
		else
		  graph[from].outgoingEdges.push_back( tmpEdge );
	  }
	}
	else if ( graph.find( from ) == graph.end() ) {
	  vertex tmpFrom;
	  tmpFrom.word = from;

	  if ( e == '.' ) 
		tmpFrom.edgesViaPeriods.push_back( tmpEdge );
	  else 
		tmpFrom.outgoingEdges.push_back( tmpEdge );
	  //(*graph).insert(pair<string, vertex>(from, tmpFrom));

	  graph[from] = tmpFrom;
	}
  }

  void addIncomingEdge( string from, string to, char e ) {
	edge tmpEdge;
	tmpEdge.e = e;
	tmpEdge.adjacent = from;
	bool alreadyExists = false;

	// if the to vertex already exists, update its incoming edges
	// if the to vertex does not exist already, make one and update its incoming edges
	if ( graph.find( to ) != graph.end() ) {
	  if ( e == '.' ) {
		for ( int j = 0; j < graph[to].edgesViaPeriods.size(); j++ ) {
		  if ( graph[to].edgesViaPeriods[j].adjacent == tmpEdge.adjacent ) {
			alreadyExists = true;
			break;
		  }
		}
	  }
	  else {
		for ( int j = 0; j < graph[to].incomingEdges.size(); j++ ) {
		  if ( graph[to].incomingEdges[j].adjacent == tmpEdge.adjacent ) {
			alreadyExists = true;
			break;
		  }
		}
	  }

	  if ( !alreadyExists ) {
		if ( e == '.' )
		  graph[to].edgesViaPeriods.push_back( tmpEdge );
		else
		  graph[to].incomingEdges.push_back( tmpEdge );
	  }
	}
	else if ( graph.find( to ) == graph.end() ) {
	  vertex tmpTo;
	  tmpTo.word = to;

	  if ( e == '.' )
		tmpTo.edgesViaPeriods.push_back( tmpEdge );
	  else
		tmpTo.incomingEdges.push_back( tmpEdge );
	  //(*graph).insert(pair<string, vertex>(to, tmpTo));

	  graph[to] = tmpTo;
	}
  }

  void constructGraph() {
	tmpString = "";

	cout << "Constructing graph ....." << endl;

	findStartingWord();

	// make graph using unique words from inputWords vector
	for ( int i = startingWordIndex + 1; i < inputWords.size(); i++ ) {

	  // find current word
	  tmpString = inputWords[i];

	  // find edge character of current word
	  if ( tmpString[tmpString.size() - 1] == '.' ) {
		currWord = tmpString.substr( 0, tmpString.size() - 1 );
		endChar = '.';	  
	  }
	  else if ( tmpString[tmpString.size() - 1] == ',' ) {
		currWord = tmpString.substr( 0, tmpString.size() - 1 );
		endChar = ',';
	  }
	  else {
		currWord = tmpString;
		endChar = '\0';
	  }

	  // add outgoing edge from left to right
	  addOutgoingEdge( prevWord, currWord, edgeChar );
	  // add incoming edge to right from left 
	  addIncomingEdge( prevWord, currWord, edgeChar );	

	  //printGraph( &graph );

	  prevWord = currWord;
	  currWord = "";
	  edgeChar = endChar;
	  endChar = '\0';
	}
	tmpString = "";

	cout << "Graph construction complete!" << endl;

	int graphSize = graph.size();
	cout << "Graph contains " << graphSize;
	if ( graphSize <= 1 )
	  cout << " vertex." << endl << endl;
	else
	  cout << " vertices." << endl << endl;
  }

  void commanateOutgoingEdge( string from, string to ) {
	for ( int i = 0; i < graph[from].outgoingEdges.size(); i++ ) {
	  if ( graph[from].outgoingEdges[i].adjacent == to ) {
		graph[from].outgoingEdges[i].e = ',';
		break;
	  }
	}
  }

  void commanateIncomingEdge( string from, string to ) {
	for ( int i = 0; i < graph[to].incomingEdges.size(); i++ ) {
	  if ( graph[to].incomingEdges[i].adjacent == from ) {
		graph[to].incomingEdges[i].e = ',';
		break;
	  }
	}
  }

  void enqueueCommanationQueue( string node, list<string>* queue, unordered_map<string, bool>* visited ) {
	for ( int j = 0; j < graph[node].outgoingEdges.size(); j++ ) {
	  string tmp = graph[node].outgoingEdges[j].adjacent;
	  if ( (*visited)[tmp] == false ) {
		queue->push_back( tmp );
		(*visited)[tmp] = true;
	  }		
	}

	for ( int j = 0; j < graph[node].incomingEdges.size(); j++ ) {
	  string tmp = graph[node].incomingEdges[j].adjacent;
	  if ( (*visited)[tmp] == false ) {
		queue->push_back( tmp );
		(*visited)[tmp] = true;
	  }		
	}

	for ( int j = 0; j < graph[node].edgesViaPeriods.size(); j++ ) {
	  string tmp = graph[node].edgesViaPeriods[j].adjacent;
	  if ( (*visited)[tmp] == false ) {
		queue->push_back( tmp );
		(*visited)[tmp] = true;
	  }	
	}
  }

  bool commanateUtil() {
	int commanateCounter = 0;
	string node = "";

	unordered_map<string, bool> visited;
	for ( auto x : graph ) {
	  visited[x.first] = false;
	}

	list<string> queue;

	visited[startingWord] = true;
	queue.push_back( startingWord );

	while ( !queue.empty() ) {
	  bool hasLeadingComma = false, hasTrailingComma = false;
	  bool incomingEdgesCommanated = true, outgoingEdgesCommanated = true;
	  int commanateCounterFlag = 0;

	  // commanate the frontmost element in the queue
	  node = queue.front();
	  queue.pop_front();	

	  // if this vertex has already been commanated, move to the next one
	  // if all incoming edges are the same (commas or spaces), the incoming side has been commanated
	  if ( graph[node].incomingEdges.size() != 0 ) {
		char incomingEdgeTmp = graph[node].incomingEdges[0].e;
		for ( int j = 1; j < graph[node].incomingEdges.size(); j++ ) {
		  if ( graph[node].incomingEdges[j].e != incomingEdgeTmp ) {
			incomingEdgesCommanated = false;
			break;
		  }		
		}
	  }	

	  // if all outgoing edges are the same (spaces or commas), the outgoing side has been commanated
	  if ( graph[node].outgoingEdges.size() != 0 ) {
		char outgoingEdgeTmp = graph[node].outgoingEdges[0].e;
		for ( int j = 1; j < graph[node].outgoingEdges.size(); j++ ) {
		  if ( graph[node].outgoingEdges[j].e != outgoingEdgeTmp ) {
			outgoingEdgesCommanated = false;
			break;
		  }
		}
	  }

	  if ( incomingEdgesCommanated && outgoingEdgesCommanated ) {
		// queue up the current vertex's adjacent vertices
		enqueueCommanationQueue( node, &queue, &visited );
		continue;
	  }


	  // check if any incoming edge has a comma
	  for ( int j = 0; j < graph[node].incomingEdges.size(); j++ ) {
		if ( graph[node].incomingEdges[j].e == ',' ) {
		  hasLeadingComma = true;
		  commanateCounterFlag++;
		  break;
		}		
	  }

	  // if so, all other incoming edges should also have commas
	  // and the vertex on the other end of that incoming vertex should have its corresponding outgoing edge updated 
	  if ( hasLeadingComma ) {
		for ( int j = 0; j < graph[node].incomingEdges.size(); j++ ) {
		  graph[node].incomingEdges[j].e = ',';
		  commanateOutgoingEdge( graph[node].incomingEdges[j].adjacent, graph[node].word );
		}
	  }

	  // check if any outgoing edge has a comma
	  for ( int j = 0; j < graph[node].outgoingEdges.size(); j++ ) {
		if ( graph[node].outgoingEdges[j].e == ',' ) {
		  hasTrailingComma = true;
		  commanateCounterFlag++;
		  break;
		}		
	  }

	  // if so, all other outgoing edges should also have commas
	  // and the vertex on the other end of that outgoing vertex should have its corresponding incoming edge updated
	  if ( hasTrailingComma ) {
		for ( int j = 0; j < graph[node].outgoingEdges.size(); j++ ) {
		  graph[node].outgoingEdges[j].e = ',';
		  commanateIncomingEdge( graph[node].word, graph[node].outgoingEdges[j].adjacent );
		}
	  }

	  if ( commanateCounterFlag == 1 || commanateCounterFlag == 2 )
		commanateCounter++;

	  // queue up the current vertex's adjacent vertices
	  enqueueCommanationQueue( node, &queue, &visited );

	}

	if ( commanateCounter == 0 )
	  return true;

	return false;
  }


public:
  DirectedGraph(string inputPath, fstream& inputFile) {
	this->inputFile = inputPath;

	readInputData( inputFile );
	constructGraph();

  }

  ~DirectedGraph() {  }

  void commanate() {
	int passNum = 1;
	do {
	  cout << "Pass #: " << passNum << endl;
	  //printGraph( &graph );
	  passNum++;
	} while ( commanateUtil() == false );
  }

  void outputCommanatedFile( string outputPath, fstream& outputFilePointer ) {
	this->outputFile = outputPath;
	tmpString = "";
	stringstream ss;

	outputFilePointer.seekp( 0 );

	for ( int i = 0; i < inputWords.size(); i++ ) {
	  tmpString = inputWords[i];

	  if ( tmpString[tmpString.size() - 1] == '.' || tmpString[tmpString.size() - 1] == ',' ) {
		tmpString.append( 1, ' ' );
		ss << tmpString;
	  }
	  else {
		if ( graph[tmpString].outgoingEdges[0].e == ',' /*|| graph[tmpString].outgoingEdges[0].e == '.'*/ )
		  tmpString.append( 1, ',' );
		tmpString.append( 1, ' ' );
		ss << tmpString;
	  }
	}
	tmpString = "";

	outputFilePointer << ss.str();
  }

  bool compareOutputFiles( string expectedPath, fstream& expectedOutputFilePointer, string ouputPath, fstream& outputFilePointer ) {
	string outputString;  
	string expectedOutputString;
	bool same = false;

	while ( expectedOutputFilePointer >> expectedOutputString ) {
	  outputFilePointer >> outputString;
	  if ( outputString == expectedOutputString ) {
		same = true;
	  }
	  else if ( outputString != expectedOutputString ) {
		same = false;
	  }
	  if ( !same )
		break;
	}
	return same;
  }

  void printGraph() {
	for ( auto x : graph ) {
	  cout << "(";
	  for ( int j = 0; j < x.second.incomingEdges.size(); j++ ) {
		if ( j != 0 )
		  cout << ", ";
		cout << "<" << x.second.incomingEdges[j].e << " [" << x.second.incomingEdges[j].adjacent << "]>";
	  }
	  cout << ") -- " << x.second.word << " -- (";
	  for ( int j = 0; j < x.second.outgoingEdges.size(); j++ ) {
		if ( j != 0 )
		  cout << ", ";
		cout << "<" << x.second.outgoingEdges[j].e << " [" << x.second.outgoingEdges[j].adjacent << "]>";
	  }
	  cout << ")" << endl;
	}
	cout << endl;
  }
};

int main() {
  auto theBigBang = high_resolution_clock::now();  

  string inputFiles[10] = {"tc-1.in", "tc-2.in", "tc-3.in",	"tc-4.in", "tc-5.in", "tc-6.in", "tc-7.in", "tc-8.in"};
  string expectedOutputFiles[10] = {"tc-1.ans", "tc-2.ans", "tc-3.ans",	"tc-4.ans", "tc-5.ans", "tc-6.ans", "tc-7.ans", "tc-8.ans"};
  string outputFiles[10] = {"result1.txt", "result2.txt", "result3.txt", "result4.txt", "result5.txt", "result6.txt", "result7.txt", "result8.txt"};
  int FUTIndex = 0; 

  for ( ; FUTIndex < 10; FUTIndex++ ) {
	if ( FUTIndex != 0 ) {
	  cout << "--------------------------------------------------------------------------------" << endl << endl;
	}

	fstream inputFilePointer( inputFiles[FUTIndex], ios::in );
	if ( !inputFilePointer ) {
	  cerr << "Could not open file: "<< inputFiles[FUTIndex] << endl;
	  return 1;
	} 

	fstream expectedOutputFilePointer( expectedOutputFiles[FUTIndex], ios::in );
	if ( !expectedOutputFilePointer ) {
	  cerr << "Could not open file: "<< expectedOutputFiles[FUTIndex] << endl;
	  return 1;
	}

	fstream outputFilePointer( outputFiles[FUTIndex], ios::out );
	if ( !outputFilePointer ) {
	  cerr << "Could not open file: "<< outputFiles[FUTIndex] << endl;
	  return 1;
	}

	cout << "Input file: " << inputFiles[FUTIndex] << endl << endl;

	DirectedGraph dGraph( inputFiles[FUTIndex], inputFilePointer );

	//cout << "Pre-commanation graph:" << endl;
	//dGraph.printGraph();

	cout << "Applying Comma Sprinkler Rules ....." << endl;

	auto start = high_resolution_clock::now();  
	dGraph.commanate();
	auto end = high_resolution_clock::now();
	int duration = duration_cast<microseconds>(end-start).count();

	cout << "Comma Sprinkler rule application complete!" << endl;
	cout << "Duration: " << fixed << setprecision( 2 ) << ((float)duration) / 1000 << "ms" << endl << endl;

	//cout << "Post-commanation graph:" << endl;
	//dGraph.printGraph();

	cout << "Generating output file ....." << endl;

	start = high_resolution_clock::now();
	dGraph.outputCommanatedFile( outputFiles[FUTIndex], outputFilePointer );
	end = high_resolution_clock::now();
	int durationTwo = duration_cast<microseconds>(end - start).count();

	cout << "Output file generated!" << endl;
	cout << "Duration: " << fixed << setprecision( 2 ) << ((float)durationTwo) / 1000 << "ms" << endl << endl;
	cout << "Net duration: " << fixed << setprecision( 2 ) << ((float)duration + durationTwo) / 1000 << "ms" << endl << endl;

	// compare output and expected output
	outputFilePointer.close();
	outputFilePointer.open( outputFiles[FUTIndex], ios::in );
	if ( !outputFilePointer ) {
	  cerr << "Could not open file: " << outputFiles[FUTIndex] << endl;
	  return 1;
	}

	cout << "Comparing output file and expected output file ....." << endl;

	bool same = dGraph.compareOutputFiles( expectedOutputFiles[FUTIndex], expectedOutputFilePointer, outputFiles[FUTIndex], outputFilePointer );

	cout << "Output file and expected output file are ";
	if ( same ) 
	  cout << "the ";
	else 
	  cout << "not the ";
	cout << "same." << endl << endl; 

	end = high_resolution_clock::now();
	int runtime = duration_cast<microseconds>(end - theBigBang).count();

	cout << "CURRENT RUNTIME: " << fixed << setprecision( 2 ) << ((float)runtime) / 1000 << "ms" << endl << endl;

	inputFilePointer.close();
	expectedOutputFilePointer.close();
	outputFilePointer.close();
  }

  return 0;
}