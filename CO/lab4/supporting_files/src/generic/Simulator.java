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
		
	static Processor processor;
	static boolean simulationComplete;
	
	public static void setupSimulation(String assemblyProgramFile, Processor p)
	{
		Simulator.processor = p;
		try{
		    loadProgram(assemblyProgramFile);
		}catch (IOException e){
		    e.printStackTrace();
		}
		
		simulationComplete = false;
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
			Clock.incrementClock();
			processor.getMAUnit().performMA();
			Clock.incrementClock();	
			processor.getEXUnit().performEX();
			Clock.incrementClock();	
			processor.getOFUnit().performOF();
			Clock.incrementClock();
			processor.getIFUnit().performIF();
			Clock.incrementClock();
			
			// setting statistics and cycle for each iteration
			int n_inst=Statistics.getNumberOfInstructions();
			Statistics.setNumberOfInstructions(n_inst+1);
			int n_cycle=Statistics.getNumberOfCycles();
			Statistics.setNumberOfCycles(n_cycle+1);
			
		}
		
		// TODO
		// set statistics
		System.out.println("Number of Cycles: " + Statistics.getNumberOfCycles());
		System.out.println("Number of OF Stalls: " + (Statistics.getNumberOfInstructions() - Statistics.getNumberOfRegisterWriteInstructions()));
		System.out.println("Number of wrong branch: " + Statistics.getNumberOfBranchTaken());
	}
	
	public static void setSimulationComplete(boolean value)
	{
		simulationComplete = value;
	}
}
