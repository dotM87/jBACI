package baci.event;

import java.util.*;
/**
 * Event Object for process changes
 * @author: David Strite
 */
public class StackChangeEvent extends EventObject {
	private int index;
	private Object oldValue;
	private Object newValue;
/**
 * Constructs a new CurrentPcodeChangeEvent with specified parameters
 * @param source
 * @param index
 * @param oldValue
 * @param newValue
 */
public StackChangeEvent(Object source, int index, Object oldValue, Object newValue) {
	super(source);
	this.index = index;
	this.oldValue = oldValue;
	this.newValue = newValue;
}
/**
 * Gets the index that was changed
 * @return int
 */
public int getIndex() {
	return index;
}
/**
 * Gets the new stack value
 * @return Object
 */
public Object getNewValue() {
	return newValue;
}
/**
 * Gets the old stack value
 * @return Object
 */
public Object getOldValue() {
	return oldValue;
}
}
