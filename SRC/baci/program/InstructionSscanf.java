package baci.program;

import java.io.*;
import baci.interpreter.*;
/**
 * InstructionSscanf
 * @author: David Strite
 */
public class InstructionSscanf extends PcodeInstruction {
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionSscanf(Program program, int x, int y) {
	super(program, x, y);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
public void doInstruction(Interpreter interpreter) {
	BaciProcess currProc = interpreter.getCurrentProcess();
	Stack stack = currProc.getStack();

	int top = currProc.getTop();
	int numAddresses = stack.getInt(top);
	Address stringAddress = stack.getAddress(top-numAddresses-1);

	String formatString = getProgram().getString(getY());
	StringBuffer searchString = new StringBuffer(stringAddress.getProcess().getStack().getString(stringAddress.getIndex()));

	int formatIndex = formatString.indexOf('%');

	Object value = null;
	int varCount = 0;
	
	while(formatIndex!=-1) {
		char type = formatString.charAt(formatIndex+1);
		trim(searchString);
		switch(type) {
			case 'd':
				value = new Integer(readNumber(searchString,false));
				break;
			case 'x':
				value = new Integer(readNumber(searchString,true));
				break;
			case 's':
				value = readString(searchString,false);
				break;
			case 'q':
				value = readString(searchString,true);
				break;
		}
		Address varAddress = stack.getAddress(top-numAddresses+varCount);
		varAddress.getProcess().getStack().set(varAddress.getIndex(),value);
		varCount++;
		formatIndex = formatString.indexOf('%',formatIndex+1);
	}
	
	currProc.setTop(top-numAddresses-1);
	stack.set(top, varCount);
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "SSCANF(&s[t-n],stab[ir.y],s[t-n-1]..s[t-1]), pop(s[t]), s[t] = # successful scans";
}
/**
 * Insert the method's description here.
 * Creation date: (9/27/2001 2:05:38 PM)
 * @return int
 * @param sb java.lang.StringBuffer
 * @param hex boolean
 */
private int readNumber(StringBuffer sb, boolean hex) {
	if(hex && sb.charAt(0)=='0' && Character.toUpperCase(sb.charAt(1))=='X') {
		sb.delete(0,2);
	}
	StringBuffer temp = new StringBuffer();
	char c = Character.toUpperCase(sb.charAt(0));
	while((c >='0' && c <='9') || (hex && c>='A' && c<='F')) {
		temp.append(c);
		sb.delete(0,1);
		if(sb.length()!=0) {
			c = Character.toUpperCase(sb.charAt(0));
		} else {
			break;
		}
	}
	return Integer.parseInt(temp.toString(),hex?16:10);
}
/**
 * Insert the method's description here.
 * Creation date: (9/27/2001 2:05:38 PM)
 * @return int
 * @param sb java.lang.StringBuffer
 * @param hex boolean
 */
private String readString(StringBuffer sb, boolean quoted) {
	StringBuffer temp = new StringBuffer();
	char c = sb.charAt(0);
	if(quoted && c=='"') {
		sb.delete(0,1);
		c = sb.charAt(0);
	}
	while((!quoted && c!=' ' && c!='\t') || (quoted && c!='"')) {
		temp.append(c);
		sb.delete(0,1);
		if(sb.length()!=0) {
			c = sb.charAt(0);
		} else {
			break;
		}
	}
	if(quoted && sb.length()>0 && c=='"') {
		sb.delete(0,1);
	}
	return temp.toString();
}
/**
 * Insert the method's description here.
 * Creation date: (9/27/2001 2:02:23 PM)
 * @param sb java.lang.StringBuffer
 */
private void trim(StringBuffer sb) {
	char c = sb.charAt(0);
	while(c==' ' || c=='\t') {
		sb.delete(0,1);
		c = sb.charAt(0);
	}
}
}
