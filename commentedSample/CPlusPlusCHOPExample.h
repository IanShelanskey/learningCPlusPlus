/* Shared Use License: This file is owned by Derivative Inc. (Derivative) and
 * can only be used, and/or modified for use, in conjunction with 
 * Derivative's TouchDesigner software, and only if you are a licensee who has
 * accepted Derivative's TouchDesigner license or assignment agreement (which
 * also govern the use of this file).  You may share a modified version of this
 * file with another authorized licensee of Derivative's TouchDesigner software.
 * Otherwise, no redistribution or sharing of this file, with or without
 * modification, is permitted.
 */




 /*
 _                                  _                   ______
 | |                                (_)                 / _____)     _      _
 | |       ____  ____   ____  ____   _  ____    ____   | /         _| |_  _| |_
 | |      / _  )/ _  | / ___)|  _ \ | ||  _ \  / _  |  | |        (_   _)(_   _)
 | |_____( (/ /( ( | || |    | | | || || | | |( ( | |  | \_____     |_|    |_|
 |_______)\____)\_||_||_|    |_| |_||_||_| |_| \_|| |   \______)
 (_____|
 Hi there,
 I am going to take the time to learn the CPlusPlus TOPs and CHOPs in TouchDesigner.
 My hope is to go through and make plain explanations of how the code is working in
 each of these examples so you can follow along and learn how these complicated objects
 work and how you can leverage them for your own systems.

 I recommend starting with learning some basics about C++ and how the logic of the language
 works. We won't be starting out with anything too complicated - however having a baseline
 understanding of C++ will be a huge benefit. Below are some helpful links:

 http://www.cppforschool.com/tutorials.html
 https://www.derivative.ca/wiki088/index.php?title=Write_a_CPlusPlus_DLL

 Derivative has left a lot of useful comments in the code already. In order parse which
 comments are which - please look for the signature <<LearnC++>> on the first line of the
 comment.

 Commentary: Ian Shelanskey [ishelanskey@gmail.com] (ianshelanskey.com)
 Date: 7/6/17

 Disclaimer: I am by no means an expert in C++ and will most likely make a mistake or two.
 If you notice any mistake please open an issue on GITHUB and I will correct it. I am doing
 this mostly to fill in the gaps in my own knowledge.

 */






/*
		<<LearnC++>>
		This is a header file. This file contains the function declarations and macro definitions
		to be shared accross several source files. 

		In this header file the developer is declaring a new object to be used in the .cpp file later.

*/

#include "CHOP_CPlusPlusBase.h"

/*
This example file implements a class that does 2 different things depending on
if a CHOP is connected to the CPlusPlus CHOPs input or not.
The example is timesliced, which is the more complex way of working.

If an input is connected the node will output the same number of channels as the
input and divide the first 'N' samples in the input channel by 2. 'N' being the current
timeslice size. This is noteworthy because if the input isn't changing then the output
will look wierd since depending on the timeslice size some number of the first samples
of the input will get used.

If no input is connected then the node will output a smooth sine wave at 120hz.
*/


// To get more help about these functions, look at CHOP_CPlusPlusBase.h


/*
		<<LearnC++>>
		Below is the object (or class) declaration. This is telling the compiler that
		there is an object called "CPlusPlusCHOPExample" which inheirits properties and
		behaviors from the class "CHOP_CPlusPlusBase". "CPlusPlusCHOPExample" is a SUBCLASS
		of "CHOP_CPlusPlusBase" as indicated by this syntax:

					class <SubClass> : public <BaseClass>
					{
						public:
						private:
					}

		This syntax allows us to use the already defined data structures and functions from
		"CHOP_CPlusPlusBase". This situation basically allows TouchDesigner to load and
		create the object in the network environment without throwing errors because we
		didn't define an important function like "execute". Instead the inheirited function
		will be called from "CHOP_CPlusPlusBase" and TouchDesigner can keep running without
		interruption.
*/

class CPlusPlusCHOPExample : public CHOP_CPlusPlusBase
{

/*
		<<LearnC++>>
		Below "public:" is where we declare all of the public functions and variables we want
		other objects and classes to have access to. Many of the one below are required for
		the CHOP to cook. TouchDesigner calls these functions at specific times in the cook cycle.


*/
public:

	//	<<LearnC++>>
	//	This is the constructor function. It is called to create a new instance of this object and allocate the defined resources specified in this class.
	CPlusPlusCHOPExample(const OP_NodeInfo* info);

	//	<<LearnC++>>
	//	This is the deconstructor function. It is called to destory the object instantiation and deallocate the defined resources specified in this class.
	virtual ~CPlusPlusCHOPExample();

	//	<<LearnC++>> 
	//  This function will always be called at the top of the cook. This allows you to set some info on how the CHOP behaves.
	//  For more information refer to line 48 and line 222 in "CPlusPlusCHOPExample.h"
	virtual void		getGeneralInfo(CHOP_GeneralInfo* ) override;

	//	<<LearnC++>>
	//	This function is called next in the cook. This function will allow you to set how many channels and samples this chop has. 
	//	For more information refer to line 93 and line 229 in "CPlusPlusCHOPExample.h"
	virtual bool		getOutputInfo(CHOP_OutputInfo*) override;

	//	<<LearnC++>>
	//	This function is called next in the cook if you have returned true in getOutputInfo. This function will allow you to set channel names according to the
	//	index passed in as argument "index". 
	//	For more information refer to line 246 in "CPlusPlusCHOPExample.h"
	virtual const char*	getChannelName(int32_t index, void* reserved) override;

	//	<<LearnC++>> 
	//	This function is called next in the cook. This is where is magic happens. By the end of this funtion you should fill CHOP_Output->channels[] with the
	//	data you want access to within TouchDesigner.
	//	For more information refer to line 134 and line 257 in "CPlusPlusCHOPExample.h" and line 257 in "CPlusPlus_Common.h"
	virtual void		execute(const CHOP_Output*,
								OP_Inputs*,
								void* reserved) override;


	//	<<LearnC++>> 
	//	This function is called next in the cook. Sets the number of info channels to be used with the info CHOP or DAT.  
	//	For more information refer to line 267 in "CPlusPlusCHOPExample.h"
	virtual int32_t		getNumInfoCHOPChans() override;
	

	//	<<LearnC++>> 
	//	This function is called next in the cook. Sets the name and value of info channels to be used with the info CHOP.  
	//	For more information refer to line 275 in "CPlusPlusCHOPExample.h"
	virtual void		getInfoCHOPChan(int index,
										OP_InfoCHOPChan* chan) override;


	//	<<LearnC++>>
	//	This function is called next in the cook. Sets the number of info channels to be used with the info DAT.  
	//	For more information refer to line 289 in "CPlusPlusCHOPExample.h"
	virtual bool		getInfoDATSize(OP_InfoDATSize* infoSize) override;
	

	//	<<LearnC++>> 
	//	This function is called next in the cook. Sets the name and value for entries to be used with the info DAT.  
	//	For more information refer to line 299 in "CPlusPlusCHOPExample.h"
	virtual void		getInfoDATEntries(int32_t index,
											int32_t nEntries,
											OP_InfoDATEntries* entries) override;


	//	<<LearnC++>> 
	//	Set up custom paramters that can be accessed within the execute call.  
	//	For more information refer to line 336 in "CPlusPlusCHOPExample.h"
	virtual void		setupParameters(OP_ParameterManager* manager) override;
	
	//	<<LearnC++>> 
	//	Called when custom pulse is pressed. 
	//	For more information refer to line 267 in "CPlusPlusCHOPExample.h"
	virtual void		pulsePressed(const char* name) override;




/*		
		<<LearnC++>>
		Below are the private variables this class will use. 
		These are variables and functions that cannot be used by any other classes. 
*/
private:

	// We don't need to store this pointer, but we do for the example.
	// The OP_NodeInfo class store information about the node that's using
	// this instance of the class (like its name).
	const OP_NodeInfo		*myNodeInfo;

	// In this example this value will be incremented each time the execute()
	// function is called, then passes back to the CHOP 
	int32_t					 myExecuteCount;


	double					 myOffset;


};
