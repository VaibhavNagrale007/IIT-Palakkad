package generic;

import java.io.PrintWriter;

public class Statistics {
	
	// TODO add your statistics here
	static int numberOfInstructions;
	static int numberOfOFStageInstructions;
	static long numberOfCycles;
	static int numberOfBranchTaken;
	static int numberOfRegisterWriteInstructions;
	static int numberOfNops;

	public static void printStatistics(String statFile)
	{
		try
		{
			PrintWriter writer = new PrintWriter(statFile);
			
			writer.println("Number of Cycles = " + numberOfCycles);
			writer.println("Number of Instructions = " + numberOfInstructions);
			writer.println("IPC = " + ((float)numberOfInstructions)/(float)numberOfCycles);
			
			// TODO add code here to print statistics in the output file
			
			writer.close();
		}
		catch(Exception e)
		{
			Misc.printErrorAndExit(e.getMessage());
		}
	}
	
	// TODO write functions to update statistics
	public static void setNumberOfInstructions(int numberOfInstructions) {
		Statistics.numberOfInstructions = numberOfInstructions;
	}

	public static int getNumberOfInstructions() {
		return numberOfInstructions;
	}

	public static void setNumberOfCycles(long numberOfCycles) {
		Statistics.numberOfCycles = numberOfCycles;
	}

	public static long getNumberOfCycles() {
		return numberOfCycles;
	}
	
	public static void setNumberOfOFInstructions(int numberOfOFStageInstructions) {
		Statistics.numberOfOFStageInstructions = numberOfOFStageInstructions;
	}
	
	public static int getNumberOfOFInstructions() {
		return numberOfOFStageInstructions;
	}
	
	public static void setNumberOfBranchTaken(int numberOfBranchTaken) {
		Statistics.numberOfBranchTaken = numberOfBranchTaken;
	}
	
	public static int getNumberOfBranchTaken() {
		return numberOfBranchTaken;
	}
	
	public static void setnumberOfRegisterWriteInstructions(int numberOfRegisterWriteInstructions) {
		Statistics.numberOfRegisterWriteInstructions = numberOfRegisterWriteInstructions;
	}
	
	public static int getNumberOfRegisterWriteInstructions() {
		return numberOfRegisterWriteInstructions;
	}
	// nop's added
	public static void setNumberOfNOPs(int numberOfNops) {
		Statistics.numberOfNops = numberOfNops;
	}
	
	public static int getNumberOfNOPs() {
		return numberOfNops;
	}
}
