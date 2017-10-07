package baci.event;

import java.util.*;
/**
 * Event Object for break point changes
 * @author: David Strite
 */
public class BreakpointChangeEvent extends EventObject {
	//Pcode address of break point
	private int pcodeIndex;
	//break point type
	private char type;
/**
 * Constructs a new BreakPointChangeEvent with specified parameters
 * @param source
 * @param pcodeIndex
 * @param type
 */
public BreakpointChangeEvent(Object source, int pcodeIndex, char type) {
	super(source);
	this.pcodeIndex = pcodeIndex;
	this.type = type;
}
/**
 * Gets the pcode address for the break point
 * @return int
 */
public int getPcodeIndex() {
	return pcodeIndex;
}
/**
 * Gets the type of the break point
 * @return char
 */
public char getType() {
	return type;
}
}
