package generic;

import java.io.FileInputStream;
import generic.Operand.OperandType;

import java.io.DataOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.FileNotFoundException;

public class Simulator {
		
	static FileInputStream inputcodeStream = null;
	static Integer addr1;
	static String outputfile=null;
	public static void setupSimulation(String assemblyProgramFile, String objectProgramFile)
	{	
		int firstCodeAddress = ParsedProgram.parseDataSection(assemblyProgramFile);
		//addr1=firstCodeAddress;
		outputfile=objectProgramFile;
		ParsedProgram.parseCodeSection(assemblyProgramFile, firstCodeAddress);
		ParsedProgram.printState();
	}
	
	public static void assemble()
	{
		//TODO your assembler code
		FileOutputStream fos=null;
		DataOutputStream dos=null;
		try{
			fos=new FileOutputStream(outputfile);
			dos=new DataOutputStream(fos);
			try{
				dos.writeInt(ParsedProgram.mainFunctionAddress);
				for(Integer data : ParsedProgram.data){
					dos.writeInt(data);
				}
				for (Instruction xyz : ParsedProgram.code){
					//Instruction xyz=ParsedProgram.code;
					switch (xyz.getOperationType()){
					case add:
				    case sub:
				    case mul:
				    case div:
				    case and:
				    case or:
				    case xor:
				    case slt:
				    case sll:
				    case srl:
				    case sra: {
				        Integer op=xyz.getOperationType().ordinal();
							Integer rs1=xyz.getSourceOperand1().getValue();
							Integer rs2=xyz.getSourceOperand2().getValue();
							Integer rd=xyz.getDestinationOperand().getValue();
							op=op<<5;op+=rs1;op=op<<5;op+=rs2;op=op<<5;op+=rd;op=op<<12;
							dos.writeInt(op);
							
							
							break;
				    		}
					case addi:
				    case subi:
				    case muli:
				    case divi:
				    case andi:
				    case ori:
				    case xori:
				    case slti:
				    case slli:
				    case srli:
				    case srai:
				    case load:
				    case store: {
							Integer op=xyz.getOperationType().ordinal();
							Integer rs1=xyz.getSourceOperand1().getValue();
							Integer rd=xyz.getDestinationOperand().getValue();
							Integer imm=xyz.getSourceOperand2().getValue();
							op=op<<5;op+=rs1;op=op<<5;op+=rd;op=op<<17;op+=imm;
							dos.writeInt(op);
							
							
							break;
						}

					case beq:
				    case bne:
				    case blt:
				    case bgt: {
				    
				    		Integer op=xyz.getOperationType().ordinal();
							Integer rd=xyz.getDestinationOperand().getValue();
							Integer address=0;
							if (xyz.getDestinationOperand().getOperandType() == OperandType.Label) {
				                address = ParsedProgram.symtab.get(xyz.getDestinationOperand().getLabelValue());
				            }
				            else {
				                address = rd;
				            }// immediate -pc
				            op=op<<5;op+=address;op=op<<22; //TODO: some correction
							dos.writeInt(op);
							
							break;
				    
				    }
						
					case jmp: {
						
							Integer op=xyz.getOperationType().ordinal();
							Integer rd=xyz.getDestinationOperand().getValue();
							Integer address=0;
							if (xyz.getDestinationOperand().getOperandType() == OperandType.Label) {
				                address = ParsedProgram.symtab.get(xyz.getDestinationOperand().getLabelValue());
				            }
				            else {
				                address = rd;
				            }// immediate -pc
				            op=op<<5;op+=address; //TODO: some correction
							dos.writeInt(op);
							
							
							break;
							
					}
					case end: {
							Integer op=xyz.getOperationType().ordinal();
							op=op<<27;
							dos.writeInt(op);
							break;
						}
					default: {
							Misc.printErrorAndExit("Undefined Instruction");
							break;
						}
					
					}
				}
				dos.flush();
				dos.close();
			}catch(IOException e){
				Misc.printErrorAndExit(e.toString());
			}
		}catch(FileNotFoundException e){
			Misc.printErrorAndExit(e.toString());
		}
		
	}
}
