package baci.program;

import baci.interpreter.*;
/**
 * InstructionSprintf
 * @author: David Strite
 */
public class InstructionSprintf extends PcodeInstruction {
	private class Format {
		public char flag;
		public int width;
		public char type;
		public int length;

		public Format(char flag, int width, char type, int length) {
			this.flag = flag;
			this.width = width;
			this.type = type;
			this.length = length;
		}
	}
/**
 * InstructionLoadAddress constructor comment.
 * @param description java.lang.String
 * @param x int
 * @param y int
 */
public InstructionSprintf(Program program, int x, int y) {
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
	StringBuffer result = new StringBuffer(formatString);

	int varCount = 0;

	int percentIndex = result.toString().indexOf('%');
	while(percentIndex!=-1) {
		Format format = getFormat(result,percentIndex);
		if(format!=null) {
			result.delete(percentIndex,percentIndex+format.length);
			switch(format.type) {
				case 'c':
				case 'd':
				case 'i':
				case 'o':
				case 'x':
				case 'X':
					result.insert(percentIndex,doNumber(stack.getInt(top-numAddresses+varCount),format));
					break;
				case 'q':
				case 's':
				case 'S':
					Address varAddress = stack.getAddress(top-numAddresses+varCount);
					String value = varAddress.getProcess().getStack().getString(varAddress.getIndex());
					result.insert(percentIndex,doString(value,format));
					break;
				
			}
			varCount++;
		}
		percentIndex = result.toString().indexOf('%',percentIndex+1);
	}

	stringAddress.getProcess().getStack().set(stringAddress.getIndex(),result.toString());
	
	currProc.setTop(top-numAddresses-2);
}
/**
 * Insert the method's description here.
 * Creation date: (9/27/2001 6:37:26 PM)
 * @return java.lang.String
 * @param sb java.lang.StringBuffer
 * @param index int
 */
private String doNumber(int number, Format format) {
	StringBuffer result = new StringBuffer();
	String value = null;
	switch(format.type) {
		case 'c':
			value = new Character((char)number).toString();
			break;
		case 'd':
		case 'i':
			value = Integer.toString(number);
			break;
		case 'o':
			value = Integer.toOctalString(number);
			if(format.flag=='#') {
				value = "0"+value;
			}
			break;
		case 'x':
		case 'X':
			value = Integer.toHexString(number);
			if(format.flag=='#') {
				value = "0x"+value;
			}
			if(format.type=='X') {
				value = value.toUpperCase();
			}
			break;
	}
	result.append(value);
	if(format.flag=='0') {
		while(result.length()<format.width) {
			result.insert(0,'0');
		}
	} else if(format.flag==0) {
		while(result.length()<format.width) {
			result.insert(0,' ');
		}
	} else {
		while(result.length()<format.width) {
			result.append(' ');
		}
	}
	return result.toString();
}
/**
 * Insert the method's description here.
 * Creation date: (9/27/2001 6:37:26 PM)
 * @return java.lang.String
 * @param sb java.lang.StringBuffer
 * @param index int
 */
private String doString(String value, Format format) {
	StringBuffer result = new StringBuffer(value);
	if(format.type=='q') {
		format.width-=2;
	}
	if(format.flag!='-') {
		while(result.length()<format.width) {
			result.insert(0,' ');
		}
	} else {
		while(result.length()<format.width) {
			result.append(' ');
		}
	}
	if(format.type=='q') {
		result.insert(0,'"');
		result.append('"');
	}
	return result.toString();
}
/**
 * Performs this Instruction's action
 * @param Interpreter calling this instruction
 */
protected String getDescription() {
	return "SPRINTF(&s[t-n],stab[ir.y],s[t-n-1]..s[t-1]),pop(s[1]+1)";
}
/**
 * Insert the method's description here.
 * Creation date: (9/27/2001 6:37:26 PM)
 * @return java.lang.String
 * @param sb java.lang.StringBuffer
 * @param index int
 */
private Format getFormat(StringBuffer sb, int index) {
	char flag;
	int width;
	char type;
	int length = 0;

	char c = sb.charAt(index);
	if(c!='%') {
		return null;
	}
	length++;
	index++;
	c= sb.charAt(index);

	if(isFlag(c)) {
		flag = c;
		index++;
		c = sb.charAt(index);
		length++;
	} else {
		flag = 0;
	}

	StringBuffer w = new StringBuffer();
	while(isWidth(c)) {
		w.append(c);
		index++;
		c = sb.charAt(index);
		length++;
	}

	if(w.length()>0) {
		width = Integer.parseInt(w.toString());
	} else {
		width = 0;
	}

	if(isType(c)) {
		type = c;
	} else {
		return null;
	}
	length++;
	
	return new Format(flag,width,type,length);
}
/**
 * Insert the method's description here.
 * Creation date: (9/27/2001 6:51:27 PM)
 * @param c char
 */
private boolean isFlag(char c) {
	switch(c) {
		case '-':
		case '+':
		case '0':
		case '#':
			return true;
		default:
			return false;
	}
}
/**
 * Insert the method's description here.
 * Creation date: (9/27/2001 6:51:27 PM)
 * @param c char
 */
private boolean isType(char c) {
	switch(c) {
		case 'c':
		case 'd':
		case 'i':
		case 'o':
		case 'u':
		case 'x':
		case 'X':
		case 's':
		case 'S':
		case 'q':
			return true;
		default:
			return false;
	}
}
/**
 * Insert the method's description here.
 * Creation date: (9/27/2001 6:51:27 PM)
 * @param c char
 */
private boolean isWidth(char c) {
	return (c>='0' && c<='9');
}
}
