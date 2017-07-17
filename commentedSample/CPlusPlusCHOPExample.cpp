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
		To start the commentary - please start in the "CPlusPlusCHOPExample.h" file.

		After reviewing the commentary in the header file procede below:

		The .cpp file is where the code that will be executed is written. For the sake of simplicity
		we can think of the header as specifing what object will be written and the .cpp file is what the
		object does. In reality, to the compiler there is very little difference between a .cpp and a .h
		and functions can be written in the .h if need as in the CHOP_CPlusPlusBase.h

		Below you will see the includes for the header files nessecary for the example, including the 
		CPlusPlusCHOPExample.h.

*/


#include "CPlusPlusCHOPExample.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>




/*
		<<LearnC++>>
		Below you will see some interesting syntax which gives us a clue how TouchDesigner uses our .DLL file. 

		Here is a simplified version so we can look at the indivdual elements:

		extern "C"                //  This tells the compiler that the functions declared here are 'C' linked. Allows TouchDesigner to call this function without sorting through the extra garbage that C++ will add. 
		{
			DLLEXPORT			  //  This is a macro which hides some more complicated / scary looking code for DLL Export. This tells the compiler that outside programs(i.e. TouchDesigner) can use this function.  
			SomeFunction()		  //  This is the actual function which will be called from the DLL.
			{	
			}
		}

		Notice that there are only three functions actually exported in the DLL:
			GetCHOPAPIVersion( void )						//Returns API Version
			CreateCHOPInstance( OP_NodeInfo )				//Returns instance of the CPlusPlusExample object we are writing.
			DestroyCHOPInstance( CHOP_CPlusPlusBase )		//Deletes the instance of the CPlusPlusExample object passed as an argument. 

		These are accessed by TouchDesigner when you create or delete a CPlusPlus CHOP that uses this DLL.  
*/


// These functions are basic C function, which the DLL loader can find
// much easier than finding a C++ Class.
// The DLLEXPORT prefix is needed so the compile exports these functions from the .dll
// you are creating
extern "C"
{

DLLEXPORT
int32_t
GetCHOPAPIVersion(void)
{
	// Always return CHOP_CPLUSPLUS_API_VERSION in this function.
	return CHOP_CPLUSPLUS_API_VERSION;
}

DLLEXPORT
CHOP_CPlusPlusBase*
CreateCHOPInstance(const OP_NodeInfo* info)
{
	// Return a new instance of your class every time this is called.
	// It will be called once per CHOP that is using the .dll
	return new CPlusPlusCHOPExample(info);
}

DLLEXPORT
void
DestroyCHOPInstance(CHOP_CPlusPlusBase* instance)
{
	// Delete the instance here, this will be called when
	// Touch is shutting down, when the CHOP using that instance is deleted, or
	// if the CHOP loads a different DLL
	delete (CPlusPlusCHOPExample*)instance;
}

};




/*
		<<LearnC++>>
		Below is where the functions for our new object are written out. These functions are not visible on the
		DLL export table, however TouchDesigner can call them after the instance of the CPlusPlusCHOPExample is created.
		This is why they need to have the same naming convention, so TouchDesigner know what to expect when 
		calling a specific function.

		I will leave comments inline to describe the process and point out specific important steps.  		
*/



//		<<LearnC++>>  This is the function definition for the constructor. Within this function we define our private variables. 
CPlusPlusCHOPExample::CPlusPlusCHOPExample(const OP_NodeInfo* info) : myNodeInfo(info)
{
	myExecuteCount = 0;
	myOffset = 0.0;
}

//		<<LearnC++>>  This is the function definition for the de-constructor. Generally nothing happens here. If you open a socket or port connection - you would close it here. 
CPlusPlusCHOPExample::~CPlusPlusCHOPExample()
{

}

//		<<LearnC++>>  This function lets you set the general info for the CHOP. 
void
CPlusPlusCHOPExample::getGeneralInfo(CHOP_GeneralInfo* ginfo)
{
	/*		
			<<LearnC++>>  ginfo is passed in as a argument. 
			Here we can alter the properties of ginfo with the following syntax:
				
				object->member = var

			In this case the object is ginfo; the members are cookEveryFrameIfAsked, timeslice, and inputMatchIndex.
			More info can be found in CHOP_CPlusPlusBase.h lines 48-90.
	*/

	// This will cause the node to cook every frame
	ginfo->cookEveryFrameIfAsked = true;
	ginfo->timeslice = true;
	ginfo->inputMatchIndex = 0;
}


//		<<LearnC++>>  This function lets you set the output info for the CHOP
bool
CPlusPlusCHOPExample::getOutputInfo(CHOP_OutputInfo* info)
{
	/*
			<<LearnC++>>  info is passed in as a argument.
			Here we can alter the properties of info with the following syntax:

			object->member = var

			The CHOP_OutputInfo object has many members. Much more info can be found in CHOP_CPlusPlusBase.h lines 93-130.
	*/

	// If there is an input connected, we are going to match it's channel names etc
	// otherwise we'll specify our own.
	if (info->opInputs->getNumInputs() > 0)
	{
		return false;
	}
	else
	{
		info->numChannels = 1;

		// Since we are outputting a timeslice, the system will dictate
		// the numSamples and startIndex of the CHOP data
		//info->numSamples = 1;
		//info->startIndex = 0

		// For illustration we are going to output 120hz data
		info->sampleRate = 120;
		return true;
	}
}


//		<<LearnC++>>  This function is called for each channel. The index of the channel should be used to switch or lookup a name.
const char*
CPlusPlusCHOPExample::getChannelName(int32_t index, void* reserved)
{
	//		<<LearnC++>> TouchDesigner will actually augment this to be chan1, chan2, chan3 when returned multiple times. 
	return "chan1";
}


/*		
		<<LearnC++>>  
		Here is the magic function. This is the definition that will return the outputs for the CHOP.
		Let's breakdown the arguments passed into this function:
			
			CHOP_Output* output			is a pointer to a CHOP_Output object. More info at CHOP_CPlusPlusBase.h lines 134-167.
			OP_Inputs* inputs			is a pointer to a OP_Inputs object. More info at CPlusPlus_Common.h lines 257-338.
			void* reserved				is for future use by Derivative.

		These objects all have members which we can get and set information to. These objects are created by TouchDesigner using the 
		previous functions defined in this .cpp and the CHOP_CPlusPlusBase.h file. In order to manipulate the outputs, we must set them to 
		something different in this function. 

		I will comment more about this inline below.

*/

void
CPlusPlusCHOPExample::execute(const CHOP_Output* output,
							  OP_Inputs* inputs,
							  void* reserved)
{
	//		<<LearnC++>>  This will increment a counter each time the execute is called. Nice debugging tool to make sure the DLL is actually working, but not essential. 
	myExecuteCount++;
	
	//		<<LearnC++>>  This is an example of how to get data from the inputs. Here we are grabbing the parameter labeled "Scale".
	double	 scale = inputs->getParDouble("Scale");

	
	/*		
			<<LearnC++>>  Below is a conditional which looks to see how many input channels there are. The there are more that 0, we will complete the 
			nested code. 
	*/
	// In this case we'll just take the first input and re-output it scaled.

	if (inputs->getNumInputs() > 0)
	{

		//		<<LearnC++>>  Below we will disable to the parameters not being used. This will prevent them from using cycles to read them.

		// We know the first CHOP has the same number of channels
		// because we returned false from getOutputInfo. 

		inputs->enablePar("Speed", 0);	// not used
		inputs->enablePar("Reset", 0);	// not used
		inputs->enablePar("Shape", 0);	// not used

		int ind = 0;

		//		<<LearnC++>>  We have two for loops here in order to iterate through each channel and each sample in the channel.
		for (int i = 0 ; i < output->numChannels; i++)
		{
			for (int j = 0; j < output->numSamples; j++)
			{
				//		<<LearnC++>>  Here we are getting the input CHOP at the first input index. 
				const OP_CHOPInput	*cinput = inputs->getInputCHOP(0);
				/*		
						<<LearnC++>>  Here we are actually setting the output channels to a scaled version of the input channels. 
						Note the syntax:
							output->channels[ channel index ][ sample index ]
						
						This is set to the input channel/sample multiplied by scale.
						"ind" is a wrapped value which is increment below if the input samples are shorter than the output samples. 
				*/
				output->channels[i][j] = float(cinput->getChannelData(i)[ind] * scale);
				//		<<LearnC++>>  Increment ind to step through the next sample.
				ind++;

				//		<<LearnC++>>  The Modulus operator wraps back around. 
				// Make sure we don't read past the end of the CHOP input
				ind = ind % cinput->numSamples;

				//		<<LearnC++>>  End of the nested loop that handles samples.
			}
			//		<<LearnC++>>  End of the loop that handles channels.
		}

	}
	//		<<LearnC++>>  Below is what happens if not inputs are connected. If inputs->getNumInputs() <= 0.
	else // If not input is connected, lets output a sine wave instead
	{
		//		<<LearnC++>>  Enable the parameters incase they were disabled before. 
		inputs->enablePar("Speed", 1);
		inputs->enablePar("Reset", 1);

		//		<<LearnC++>>  Grab the parameter labeled "Speed"
		double speed = inputs->getParDouble("Speed");
		double step = speed * 0.01f;


		// menu items can be evaluated as either an integer menu position, or a string
		int shape = inputs->getParInt("Shape");
//		const char *shape_str = inputs->getParString("Shape");

		// keep each channel at a different phase
		double phase = 2.0f * 3.14159f / (float)(output->numChannels);

		// Notice that startIndex and the output->numSamples is used to output a smooth
		// wave by ensuring that we are outputting a value for each sample
		// Since we are outputting at 120, for each frame that has passed we'll be
		// outputing 2 samples (assuming the timeline is running at 60hz).

		//		<<LearnC++>>  This should look familiar to what is above. Two for loops, one for channel and one for samples. 
		for (int i = 0; i < output->numChannels; i++)
		{
			double offset = myOffset + phase*i;


			double v = 0.0f;
			//		<<LearnC++>>  This switch case detirmines the shape the waveform will have. This is grabbed from a paramter on line 321.
			switch(shape)
			{
				case 0:		// sine
					v = sin(offset);
					break;

				case 1:		// square
					v = fabs(fmod(offset, 1.0)) > 0.5;
					break;

				case 2:		// ramp	
					v = fabs(fmod(offset, 1.0));
					break;
			}


			v *= scale;

			for (int j = 0; j < output->numSamples; j++)
			{
				//		<<LearnC++>>  Data is passed into the channels/samples here.
				output->channels[i][j] = float(v);
				offset += step;
			}
		}

		myOffset += step * output->numSamples; 
	}
	/*
			<<LearnC++>>
			It's important to remember that this function is called each time the operator cooks.
			To avoid memory overflow or other more serious problems - be sure to deallocate any memory you might have used before returning.
			This example does not use any complex data structures and will reuse the same memory each time its called so it will not overflow. 

			Another important thing to remember is that memory is not guarenteed to persist to the next time the function is called. If you need this 
			type of capability create a private variable the belongs to the class and set it before returning. You can access it on the next function call. 

	*/
}

int32_t
CPlusPlusCHOPExample::getNumInfoCHOPChans()
{
	// We return the number of channel we want to output to any Info CHOP
	// connected to the CHOP. In this example we are just going to send one channel.
	return 2;
}

void
CPlusPlusCHOPExample::getInfoCHOPChan(int32_t index,
										OP_InfoCHOPChan* chan)
{
	// This function will be called once for each channel we said we'd want to return
	// In this example it'll only be called once.

	if (index == 0)
	{
		chan->name = "executeCount";
		chan->value = (float)myExecuteCount;
	}

	if (index == 1)
	{
		chan->name = "offset";
		chan->value = (float)myOffset;
	}
}

bool		
CPlusPlusCHOPExample::getInfoDATSize(OP_InfoDATSize* infoSize)
{
	infoSize->rows = 2;
	infoSize->cols = 2;
	// Setting this to false means we'll be assigning values to the table
	// one row at a time. True means we'll do it one column at a time.
	infoSize->byColumn = false;
	return true;
}

void
CPlusPlusCHOPExample::getInfoDATEntries(int32_t index,
										int32_t nEntries,
										OP_InfoDATEntries* entries)
{
	// It's safe to use static buffers here because Touch will make it's own
	// copies of the strings immediately after this call returns
	// (so the buffers can be reuse for each column/row)
	static char tempBuffer1[4096];
	static char tempBuffer2[4096];

	if (index == 0)
	{
		// Set the value for the first column
#ifdef WIN32
		strcpy_s(tempBuffer1, "executeCount");
#else // macOS
        strlcpy(tempBuffer1, "executeCount", sizeof(tempBuffer1));
#endif
		entries->values[0] = tempBuffer1;

		// Set the value for the second column
#ifdef WIN32
		sprintf_s(tempBuffer2, "%d", myExecuteCount);
#else // macOS
        snprintf(tempBuffer2, sizeof(tempBuffer2), "%d", myExecuteCount);
#endif
		entries->values[1] = tempBuffer2;
	}

	if (index == 1)
	{
		// Set the value for the first column
#ifdef WIN32
        strcpy_s(tempBuffer1, "offset");
#else // macOS
        strlcpy(tempBuffer1, "offset", sizeof(tempBuffer1));
#endif
		entries->values[0] = tempBuffer1;

		// Set the value for the second column
#ifdef WIN32
        sprintf_s(tempBuffer2, "%g", myOffset);
#else // macOS
        snprintf(tempBuffer2, sizeof(tempBuffer2), "%g", myOffset);
#endif
		entries->values[1] = tempBuffer2;
	}
}

void
CPlusPlusCHOPExample::setupParameters(OP_ParameterManager* manager)
{
	// speed
	{
		OP_NumericParameter	np;

		np.name = "Speed";
		np.label = "Speed";
		np.defaultValues[0] = 1.0;
		np.minSliders[0] = -10.0;
		np.maxSliders[0] =  10.0;
		
		OP_ParAppendResult res = manager->appendFloat(np);
		assert(res == OP_ParAppendResult::Success);
	}

	// scale
	{
		OP_NumericParameter	np;

		np.name = "Scale";
		np.label = "Scale";
		np.defaultValues[0] = 1.0;
		np.minSliders[0] = -10.0;
		np.maxSliders[0] =  10.0;
		
		OP_ParAppendResult res = manager->appendFloat(np);
		assert(res == OP_ParAppendResult::Success);
	}

	// shape
	{
		OP_StringParameter	sp;

		sp.name = "Shape";
		sp.label = "Shape";

		sp.defaultValue = "Sine";

		const char *names[] = { "Sine", "Square", "Ramp" };
		const char *labels[] = { "Sine", "Square", "Ramp" };

		OP_ParAppendResult res = manager->appendMenu(sp, 3, names, labels);
		assert(res == OP_ParAppendResult::Success);
	}

	// pulse
	{
		OP_NumericParameter	np;

		np.name = "Reset";
		np.label = "Reset";
		
		OP_ParAppendResult res = manager->appendPulse(np);
		assert(res == OP_ParAppendResult::Success);
	}

}

void 
CPlusPlusCHOPExample::pulsePressed(const char* name)
{
	if (!strcmp(name, "Reset"))
	{
		myOffset = 0.0;
	}
}

