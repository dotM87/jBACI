package baci.event;

import java.util.*;
/**
 * Event Object for process changes
 * @author: David Strite
 */
public class ProcessChangeEvent extends EventObject {
	private int property;
	private Object oldValue;
	private Object newValue;

	public static final int TOP = 0;
	public static final int BOTTOM = 1;
	public static final int ACTIVE = 2;
	public static final int FINISHED = 3;
	public static final int SUSPEND = 4;
	public static final int MONITOR = 5;
	public static final int PRIORITY = 6;
	public static final int ATOMIC = 7;
/**
 * Constructs a new CurrentPcodeChangeEvent with specified parameters
 * @param source
 * @param property
 * @param oldValue
 * @param newValue
 */
public ProcessChangeEvent(Object source,int property, Object oldValue, Object newValue) {
	super(source);
	this.property = property;
	this.oldValue = oldValue;
	this.newValue = newValue;
}
/**
 * Gets new property value
 * @return Object
 */
public Object getNewValue() {
	return newValue;
}
/**
 * Gets the old property value
 * @return Object
 */
public Object getOldValue() {
	return oldValue;
}
/**
 * Gets the property that was changed
 * @return int
 */
public int getProperty() {
	return property;
}
}
