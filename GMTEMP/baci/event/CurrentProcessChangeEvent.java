package baci.event;

import java.util.*;
/**
 * Event Object for current process changes
 * @author: David Strite
 */
public class CurrentProcessChangeEvent extends EventObject {
	private int newCurrent;
	private int oldCurrent;
/**
 * Constructs a new CurrentPcodeChangeEvent with specified parameters
 * @param source
 * @param oldCurrent
 * @param newCurrent
 */
public CurrentProcessChangeEvent(Object source, int oldCurrent, int newCurrent) {
	super(source);
	this.oldCurrent = oldCurrent;
	this.newCurrent = newCurrent;
}
/**
 * Gets the new current process index
 * @return int
 */
public int getNewCurrent() {
	return newCurrent;
}
/**
 * Gets the old current process index
 * @return int
 */
public int getOldCurrent() {
	return oldCurrent;
}
}
