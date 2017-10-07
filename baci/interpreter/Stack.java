package baci.interpreter;

import baci.event.*;
import java.util.*;
/**
 * Represents a stack
 * @author: David Strite
 */

public class Stack {
	//stack data
	private ArrayList stack;
	//listeners
	private ArrayList listeners;
/**
 * Constructs a new Stack
 */
public Stack() {
	stack = new ArrayList();
	listeners = new ArrayList();
}
/**
 * Add a listener to the list that's notified when this Stack changes
 * @param l listener
 */
public void addStackListener(StackListener l) {
	listeners.add(l);
}
/**
 * Notify listeners that this Stack has changed
 * @param event
 */
private void fireStackChange(StackChangeEvent event) {
	for(int i=listeners.size()-1; i>=0; i--) {
		((StackListener)listeners.get(i)).stackChanged(event);
	}
}
/**
 * Gets the object as specified index
 * @param index
 * @return Object
 */
public Object get(int index) {
	try {
		return stack.get(index);
	} catch(IndexOutOfBoundsException e) {
		return null;
	}
}
/**
 * Gets the Address as specified index
 * @param index
 * @return Address
 */
public Address getAddress(int index) {
	return (Address)get(index);
}
/**
 * Gets the int as specified index
 * @param index
 * @return int
 */
public int getInt(int index) {
	Integer value = (Integer)get(index);
	return value!=null?value.intValue():0;
}
/**
 * Gets the String as specified index
 * @param index
 * @return String
 */
public String getString(int index) {
	return (String)get(index);
}
/**
 * Remove a listener from the list that's notified when this Stack changes
 * @param l listener
 */
public void removeStackListener(StackListener l) {
	listeners.remove(l);
}
/**
 * Sets value at specified index
 * @param index
 * @param value
 */
public void set(int index, int value) {
	set(index,new Integer(value));
}
/**
 * Sets value at specified index
 * @param index
 * @param value
 */
public void set(int index, Address value) {
	set(index,(Object)value);
}
/**
 * Sets value at specified index
 * @param index
 * @param value
 */
public void set(int index, Object value) {
	while(stack.size()<=index) {
		stack.add(null);
	}
	Object oldValue = stack.get(index);
	stack.set(index,value);
	fireStackChange(new StackChangeEvent(this,index,oldValue,value));
}
/**
 * Sets value at specified index
 * @param index
 * @param value
 */
public void set(int index, String value) {
	set(index,(Object)value);
}
}
