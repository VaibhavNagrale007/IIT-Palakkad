package generic;

import processor.Clock;
import processor.Processor;

import java.io.FileInputStream;
import java.io.DataInputStream;
import java.io.InputStream;
import generic.Statistics;
import java.io.IOException;
import java.io.FileNotFoundException;

public class Simulator {
		
	static EventQueue eventQueue;
	static Processor processor;
	static boolean simulationComplete;
	
	public static EventQueue getEventQueue()
	{
		return eventQueue;
	}
	
	public static void setupSimulation(String assemblyProgramFile, Processor p)
	{
		Simulator.processor = p;
		try{
		    loadProgram(assemblyProgramFile);
		}catch (IOException e){
		    e.printStackTrace();
		}
		
		simulationComplete = false;
		eventQueue = new EventQueue();
	}
	
	static void loadProgram(String assemblyProgramFile) throws IOException
	{
		/*
		 * TODO
		 * 1. load the program into memory according to the program layout described
		 *    in the ISA specification
		 * 2. set PC to the address of the first instruction in the main
		 * 3. set the following registers:
		 *     x0 = 0
		 *     x1 = 65535
		 *     x2 = 65535
		 */
		 
		 InputStream Inp=null;
		 try{
		 	Inp=new FileInputStream(assemblyProgramFile);
		 	
		 }catch(FileNotFoundException e){
		 	e.printStackTrace();
		 }
		 DataInputStream Data_Inp=new DataInputStream(Inp);
		 
		 //2. reading data and setting program counter
		 // processor.getRegisterFile().setProgramCounter(Data_Inp.readInt());
		 //reading until available
		 int addr=-1;	// address 
		 while(Data_Inp.available()>0){			//( address, value )
		    int next=Data_Inp.readInt();
		    if (addr==-1){
		        processor.getRegisterFile().setProgramCounter(next);
		    }else{
		        processor.getMainMemory().setWord(addr,next);
		    }
		 	
		 	// memory[address]=value
		 	addr+=1;
		 }
		 //3. setting value to registers
		 processor.getRegisterFile().setValue(0,0);
		 processor.getRegisterFile().setValue(1,65535);
		 processor.getRegisterFile().setValue(2,65535);
	}
	
	public static void simulate()
	{
		while(simulationComplete == false)
		{
			processor.getRWUnit().performRW();
			//Clock.incrementClock();
			processor.getMAUnit().performMA();
			//Clock.incrementClock();	
			processor.getEXUnit().performEX();
			eventQueue.processEvents();
			//Clock.incrementClock();	
			processor.getOFUnit().performOF();
			//Clock.incrementClock();
			processor.getIFUnit().performIF();
			Clock.incrementClock();
			
		}
		
		// TODO
		// set statistics
		// setting statistics and cycle for each iteration
		int n_inst=Statistics.getNumberOfInstructions();
		Statistics.setNumberOfInstructions(n_inst);
		long n_cycle=Clock.getCurrentTime();
		Statistics.setNumberOfCycles(n_cycle);
		
		// printing reports
		System.out.println("Number of Cycles: " + Statistics.getNumberOfCycles());
		System.out.println("Number of Instructions: " + Statistics.getNumberOfInstructions());
		System.out.println("IPC: " + ((float)Statistics.getNumberOfInstructions() / (float)Statistics.getNumberOfCycles()));
	}
	
	public static void setSimulationComplete(boolean value)
	{
		simulationComplete = value;
	}
}
