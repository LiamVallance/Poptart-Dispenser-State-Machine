/*******************************************************
* Poptart dispenser Machine.
*
* Created By:
* Liam Vallance February/March 2017
* Based On:
* Coursework Apendix A/State Diagram
*
* //LV// <- Precedes comments made by Liam Vallance
******************************************************/

#include<iostream>
#include<string>
#include<vector>

using namespace std;

enum state { Out_Of_Poptart, No_Credit, Has_Credit, Dispenses_Poptart }; //LV//The four possible states the machine can be in as outlined within the state diagram
enum stateParameter { No_Of_Poptarts, Credit, Cost_Of_Poptart }; //LV//State Parameter pointer values held in a vecter list, used as a middle man to get and set parameter values but don't hold the values themselfs

class StateContext;

#pragma region Apendix Code

class State
{
protected:

	StateContext* CurrentContext;

public:

	State(StateContext* Context)
	{
		CurrentContext = Context;
	}
	virtual ~State(void) {}
	virtual void transition(void) {}
};

class StateContext
{
protected:
	
	State* CurrentState = nullptr;
	int stateIndex = 0; //LV//stateIndex defaulted to 0 used for position in the available state vector
	vector<State*> availableStates; //LV//availableStates vector holding the Out_Of_Poptart, No_Credit, Has_Credit, Dispenses_Poptart sates
	vector<int> stateParameters; //LV//statePerameter vector which holds No_Of_Poptarts, Credit, Cost_Of_Poptart

public:	//LV//StateContect publics declared for later use with expected pass in types declared
	virtual ~StateContext(void);
	virtual void setState(state newState);
	virtual int getStateIndex(void);
	virtual void setStateParam(stateParameter SP, int value);
	virtual int getStateParam(stateParameter SP);
};

StateContext::~StateContext(void)
{	//LV//Cleans out availableStates and stateParameters vectors
	for (int i = 0; i < this->availableStates.size(); i++) delete this->availableStates[i];
	this->availableStates.clear();
	this->stateParameters.clear();
}

void StateContext::setState(state newState)
{	//LV//Sets current state to the current newState in availableStates, sets the stateIndex to the newState and the CurrentStates transition
	this->CurrentState = availableStates[newState];
	this->stateIndex = newState;
	this->CurrentState->transition();
}

int StateContext::getStateIndex(void)
{	//LV//Used to get the stateIndex through function call
	return this->stateIndex;
}

void StateContext::setStateParam(stateParameter SP, int value)
{	//LV//Used throughout program through function call to set the stateParameter to the value specified in the function call
	this->stateParameters[SP] = value;
}

int StateContext::getStateParam(stateParameter SP)
{	//LV//Used to get the stateParameters through function call
	return this->stateParameters[SP];
}

class Transition //Default error catch if no definition is present but a call is attempted
{
public: //LV//Should not be called as error calls giving more information set in state function definitions
	virtual bool insertMoney(int) { cout << "Error!" << endl; return false; }
	virtual bool makeSelection(int) { cout << "Error!" << endl; return false; }
	virtual bool moneyRejected(void) { cout << "Error!" << endl; return false; }
	virtual bool addPoptart(int) { cout << "Error!" << endl; return false; }
	virtual bool dispense(void) { cout << "Error!" << endl; return false; }
};

class PoptartState : public State, public Transition
{
public:
	PoptartState(StateContext* Context) : State(Context) {}
};
//LV//OutOfPoptart class deffinitions with function call expected types
class OutOfPoptart : public PoptartState
{
public:
	OutOfPoptart(StateContext* Context) : PoptartState(Context) {}
	bool insertMoney(int money);
	bool makeSelection(int option);
	bool moneyRejected(void);
	bool addPoptart(int number);
	bool dispense(void);
};
//LV//NoCredit class deffinitions with function call expected types
class NoCredit : public PoptartState
{
public:
	NoCredit(StateContext* Context) : PoptartState(Context) {}
	bool insertMoney(int money);
	bool makeSelection(int option);
	bool moneyRejected(void);
	bool addPoptart(int number);
	bool dispense(void);
};
//LV//HasCredit class deffinitions with function call expected types
class HasCredit : public PoptartState
{
public:
	HasCredit(StateContext* Context) : PoptartState(Context) {}
	bool insertMoney(int money);
	bool makeSelection(int option);
	bool moneyRejected(void);
	bool addPoptart(int number);
	bool dispense(void);
};
//LV//DispensesPoptart class deffinitions with function call expected types
class DispensesPoptart : public PoptartState
{
public:
	DispensesPoptart(StateContext* Context) : PoptartState(Context) {}
	bool insertMoney(int money);
	bool makeSelection(int option);
	bool moneyRejected(void);
	bool addPoptart(int number);
	bool dispense(void);
};

class Product
{
protected:
	string product_description;
	int itemCost = 0;
public:
	virtual int cost(void); //LV//Public variable used through function call to access itemCost protected variable
	virtual string description(void); //LV//Public variable used through function call to access product_description protected variable
};

class Poptart : public Product //LV//Used to create Poptart as a new Product and inherit the Product super class attributes
{
public:
	Poptart(void);
};

class Poptart_Dispenser : public StateContext, public Transition
{
	friend class DispensesPoptart;
	friend class HasCredit;
private:
	PoptartState* PoptartCurrentState = nullptr;
	bool itemDispensed = false; //indicates whether a product is there to be reteived
	Product* DispensedItem = nullptr;
	bool itemRetrieved = false; //indicates whether a product has been retrieved
public:
	Poptart_Dispenser(int inventory_count);
	~Poptart_Dispenser(void);
	bool insertMoney(int money); //LV//Used with main to add money to the machine memory
	bool makeSelection(int option); //LV//Used with main to make selection based on option code passed in
	bool moneyRejected(void); //LV//Used with main to reject money
	bool addPoptart(int number); //LV//Used with main to add the number of poptarts passed in to the machine memory
	bool dispense(void); //LV//Used with main to dispense product
	Product* getProduct(void);
	virtual void setStateParam(stateParameter SP, int value); //LV//Used to set the stateParameter to the value specified
	virtual int getStateParam(stateParameter SP); //LV//Used to get the stateParameter
};

Poptart_Dispenser::~Poptart_Dispenser(void)
{
	if (!this->itemRetrieved)
	{	//LV//DispensedItem cleanup
		delete this->DispensedItem;
	}
}

bool Poptart_Dispenser::insertMoney(int money)
{
	PoptartCurrentState = (PoptartState*) this->CurrentState;
	return this->PoptartCurrentState->insertMoney(money);
}

bool Poptart_Dispenser::makeSelection(int option)
{
	PoptartCurrentState = (PoptartState*) this->CurrentState;
	return this->PoptartCurrentState->makeSelection(option);;
}

bool Poptart_Dispenser::moneyRejected(void)
{
	PoptartCurrentState = (PoptartState*) this->CurrentState;
	return this->PoptartCurrentState->moneyRejected();
}

bool Poptart_Dispenser::addPoptart(int number)
{
	PoptartCurrentState = (PoptartState*) this->CurrentState;
	return this->PoptartCurrentState->addPoptart(number);
}

bool Poptart_Dispenser::dispense(void)
{
	PoptartCurrentState = (PoptartState*) this->CurrentState;
	return this->PoptartCurrentState->dispense();
}

Product* Poptart_Dispenser::getProduct(void)
{
	if (this->itemDispensed)
	{	//LV//If the item is dispensed then set the item to retrieved and return the item dispensed
		this->itemDispensed = false;
		this->itemRetrieved = true;
		return this->DispensedItem;
	}

	return nullptr;
}

void Poptart_Dispenser::setStateParam(stateParameter SP, int value)
{	//LV//If the current state parameter is not Cost_Of_Poptart then allow stateParameter passed in to be value passed int
	if (SP == Cost_Of_Poptart) return;
	this->stateParameters[SP] = value;
}

int Poptart_Dispenser::getStateParam(stateParameter SP)
{
	if (SP == Cost_Of_Poptart)
	{	//LV//If stateParameter is Cost_Of_Poptart and DispensedItem selected then return DispensedItem cost and return stateParameter value
		if (DispensedItem == nullptr) return 0;
		return DispensedItem->cost();
	}
	return this->stateParameters[SP];
}
#pragma endregion
///////////////////////////////////////////
////////////End of Apendix Code////////////
///////////////////////////////////////////


//LV//Provides link access to the protected value itemCost via the cost public variable
int Product::cost(void)
{
	return this->itemCost;
}
//LV//Provides link access to the protected value product_description via the description public variable
string Product::description(void)
{
	return this->product_description;
}

Poptart::Poptart(void)
{
}

Poptart_Dispenser::Poptart_Dispenser(int inventory_count)
{
	this->availableStates.push_back(new OutOfPoptart(this));
	this->availableStates.push_back(new NoCredit(this));
	this->availableStates.push_back(new HasCredit(this));
	this->availableStates.push_back(new DispensesPoptart(this));
	this->stateParameters.push_back(0);// Number of Poptarts
	this->stateParameters.push_back(0);// Available Credit

	this->setState(Out_Of_Poptart);
	if (inventory_count > 0)
	{
		this->addPoptart(inventory_count);
	}
}
//LV//Poptart Base classes of type Poptart holding product_description and itemCost for each base accessed through the cost and description Product functions
#pragma region Poptart Bases
class Plain : public Poptart
{
public:
	Plain(void)
	{
		product_description = "Plain Poptart!";
		itemCost = 100;
	}
};

class Spicy : public Poptart
{
public:
	Spicy(void)
	{
		product_description = "Spicy Poptart!";
		itemCost = 150;
	}
};

class Chocolate : public Poptart
{
public:
	Chocolate(void)
	{
		product_description = "Chocolate Poptart!";
		itemCost = 200;
	}
};

class Coconut : public Poptart
{
public:
	Coconut(void)
	{
		product_description = "Coconut Poptart!";
		itemCost = 200;
	}
};

class Fruity : public Poptart
{
public:
	Fruity(void)
	{
		product_description = "Fruity Poptart!";
		itemCost = 200;
	}
};
#pragma endregion

//LV//Class function definitions for changing state and updating state parameters
//LV//Based upon the state diagram given in the coursework sheet
#pragma region OutOfPoptart Function Definitions
bool OutOfPoptart::addPoptart(int number)
{	//LV//Adds the number of poptarts added to the machine to machines memory of poptarts held and transitions from Out_Of_Poptart state to No_Credit state
	cout << "Added Poptarts: " << number << endl;
	this->CurrentContext->setStateParam(No_Of_Poptarts, number); 
	this->CurrentContext->setState(No_Credit);
	
	return true;
}

bool OutOfPoptart::moneyRejected(void)
{
	cout << "Credit rejected out of poptarts" << endl;
	return false;
}
#pragma region OutOfPoptart Error Catch
bool OutOfPoptart::insertMoney(int money)
{
	cout << "OutOfPoptart, cannot insert money!" << endl;
	return false;
}

bool OutOfPoptart::makeSelection(int option)
{
	cout << "OutOfPoptart, cannot make selection!" << endl;
	return false;
}

bool OutOfPoptart::dispense(void)
{
	cout << "OutOfPoptart, cannot dispense!" << endl;
	return false;
}
#pragma endregion
#pragma endregion

#pragma region NoCredit Function Definitions
bool NoCredit::insertMoney(int money)
{	//LV//Sets the available credit to the value that the user inserts and transitions from the No_Credit state to the Has_Credit state
	this->CurrentContext->setStateParam(Credit, money);
	cout << "Available Credit: " << this->CurrentContext->getStateParam(Credit) << endl;
	this->CurrentContext->setState(Has_Credit);

	return true;
}
#pragma region NoCredit Error Catch
bool NoCredit::makeSelection(int option)
{
	cout << "NoCredit, cannot make selection!" << endl;
	return false;
}

bool NoCredit::moneyRejected()
{
	cout << "NoCredit, cannot reject money!" << endl;
	return false;
}

bool NoCredit::addPoptart(int number)
{
	cout << "NoCredit, cannot add poptart!" << endl;

	return false;
}

bool NoCredit::dispense()
{
	cout << "NoCredit, cannot dispense!" << endl;
	return false;
}
#pragma endregion
#pragma endregion

#pragma region HasCredit Function Definitions
bool HasCredit::insertMoney(int money)
{	//LV//Increases the users credit by the amount input instead of overwriting the current available credit
	this->CurrentContext->setStateParam(Credit, this->CurrentContext->getStateParam(Credit) + money);
	cout << "Available Credit: " << this->CurrentContext->getStateParam(Credit) << endl;
	
	return true;
}

bool HasCredit::makeSelection(int option)
{	//LV//Selects base depending on user selection with error catch incase incorrect selection is possible
	Poptart* RequestedPoptart;

	this->CurrentContext->setStateParam(Cost_Of_Poptart, option);

		if (option == 1)
			RequestedPoptart = new Plain;
		else if (option == 2)
			RequestedPoptart = new Spicy;
		else if (option == 4)
			RequestedPoptart = new Chocolate;
		else if (option == 8)
			RequestedPoptart = new Coconut;
		else if (option == 16)
			RequestedPoptart = new Fruity;
		else {
			RequestedPoptart = new Poptart;
			cout << "Invalid Selection!" << endl;
			return false;
			}
		//LV//Grabs description and cost values relevent to the base type the user selected
	cout << "Selection made: " << RequestedPoptart->description() << endl;
	cout << "Cost of product: " << RequestedPoptart->cost() << endl;
	//LV//Checks if the user has enough credit for their selection, if so deducts the cost from the users credit and transitions from Has_Credit state to Dispenses_Poptart state
	if (this->CurrentContext->getStateParam(Credit) >= RequestedPoptart->cost())
	{
		this->CurrentContext->setStateParam(Credit, this->CurrentContext->getStateParam(Credit) - RequestedPoptart->cost());
		this->CurrentContext->setState(Dispenses_Poptart);
	}
	else { cout << "Not enough money!" << endl; }

	return true;
}

bool HasCredit::moneyRejected(void)
{	//LV//Dispenses users money, sets avaiable credit in memory to be 0 and transitions from Has_Credit state to No_Credit
	cout << "Credit rejected" << endl;
	this->CurrentContext->setStateParam(Credit, this->CurrentContext->getStateParam(Credit) - this->CurrentContext->getStateParam(Credit));
	this->CurrentContext->setState(No_Credit);
	
	return true;
}
#pragma region HasCredit Error Catch
bool HasCredit::addPoptart(int number)
{
	cout << "HasCredit, cannot add poptart!" << endl;
	return false;
}

bool HasCredit::dispense(void)
{
	cout << "HasCredit, cannot dispense!" << endl;
	return false;
}
#pragma endregion
#pragma endregion

#pragma region DispensesPoptart Function Definitions
bool DispensesPoptart::dispense(void)
{
	cout << "Poptarts dispensed" << endl;
	//LV//Decrements the number of poptarts the mechine is holding in memory each time a phisical one is dispensed
	this->CurrentContext->setStateParam(No_Of_Poptarts, this->CurrentContext->getStateParam(No_Of_Poptarts) - 1);
	//LV//Transitions from current Dispenses_Poptart state to Out_Of_Poptart state if machine is empty
	if (this->CurrentContext->getStateParam(No_Of_Poptarts) == 0) 
	 this->CurrentContext->setState(Out_Of_Poptart); 
	//LV//Transitions from current Dispenses_Poptart state to No_Credit state if machine is not empty and users has no more credit
	else if (this->CurrentContext->getStateParam(No_Of_Poptarts) > 0 && this->CurrentContext->getStateParam(Credit) == 0)
	 this->CurrentContext->setState(No_Credit); 
	//LV//Transitions from current Dispenses_Poptart state to Has_Credit state if machine is not empty and users still has available credit
	else if (this->CurrentContext->getStateParam(No_Of_Poptarts) > 0 && this->CurrentContext->getStateParam(Credit) > 0)
	 this->CurrentContext->setState(Has_Credit); 
	
	return true;
}
#pragma region DispensesPoptart Error Catch
bool DispensesPoptart::insertMoney(int money)
{
	cout << "Dispensing, cannot insert money!" << endl;
	return false;
}

bool DispensesPoptart::makeSelection(int option)
{
	cout << "Dispensing, cannot make selection!" << endl;
	return false;
}

bool DispensesPoptart::moneyRejected(void)
{
	cout << "Dispensing, cannot reject money!" << endl;
	return false;
}

bool DispensesPoptart::addPoptart(int number)
{
	cout << "Dispensing, cannot add poptart!" << endl;
	return false;
}
#pragma endregion
#pragma endregion



void main()
{
	cout << "Poptart Machine" << endl;
	Poptart_Dispenser test = Poptart_Dispenser(50);

	test.insertMoney(200);
	test.makeSelection(8);
	test.dispense();

	system("pause");
}