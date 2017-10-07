package baci.event;

import java.util.*;
/**
 * Event Object for current pcode address changes
 * @author: David Strite
 */
public class CurrentPcodeChangeEvent extends EventObject {
	private int oldCurrentPcode;
	private int newCurrentPcode;
/**
 * Constructs a new CurrentPcodeChangeEvent with specified parameters
 * @param source
 * @param oldCP
 * @param newCP
 */
public CurrentPcodeChangeEvent(Object source, int oldCP, int newCP) {
	super(source);
	oldCurrentPcode = oldCP;
	newCurrentPcode = newCP;
}
/**
 * Gets the new current pcode address
 * @return int
 */
public int getNewCurrentPcode() {
	return newCurrentPcode;
}
/**
 * Gets the old current pcode address
 * @return int
 */
public int getOldCurrentPcode() {
	return oldCurrentPcode;
}
}
