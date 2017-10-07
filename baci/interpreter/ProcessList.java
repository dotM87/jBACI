package baci.interpreter;

import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
/**
 * Stores list of processes
 * @author: David Strite
 */
public class ProcessList implements ListModel {
	//List of processes
	private ArrayList processes;
	//List of listeners
	private ArrayList listeners;
/**
 * Constructs a new ProcessList
 */
public ProcessList() {
	processes = new ArrayList();
	listeners = new ArrayList();
}
/**
 * Adds a process to the list
 * @param p process to add
 */
public void add(BaciProcess p) {
	int index = processes.size();
	processes.add(p);
	fireIntervalAdded(this,index,index);
}
/**
  * Adds a listener to the list that's notified when the lsit changes
  * @param l listener
  */
public void addListDataListener(ListDataListener l) {
	listeners.add(l);
}
/**
 * Removes all processes from list
 */
public void clear() {
	int index = processes.size()-1;
	processes.clear();
	if (index >= 0) {
	    fireIntervalRemoved(this,0,index);
	}

}
/**
 * Notifies listeners when list elements are changed
 * @param source
 * @param index0
 * @param index1
 */
private void fireContentsChanged(Object source, int index0, int index1) {
	ListDataEvent e = null;

	for (int i=listeners.size()-1; i>=0; i--) {
		if (e == null) {
		    e = new ListDataEvent(source, ListDataEvent.CONTENTS_CHANGED, index0, index1);
		}
		((ListDataListener)listeners.get(i)).contentsChanged(e);
	}
}
/**
 * Notifies listeners when items are added to list
 * @param source
 * @param index0
 * @param index1
 */
private void fireIntervalAdded(Object source, int index0, int index1) {
	ListDataEvent e = null;

	for (int i=listeners.size()-1; i>=0; i--) {
		if (e == null) {
		    e = new ListDataEvent(source, ListDataEvent.INTERVAL_ADDED, index0, index1);
		}
		((ListDataListener)listeners.get(i)).intervalAdded(e);
	}
}
/**
 * Notifies listeners when elements are removed from list
 * @param source
 * @param index0
 * @param index1
 */
private void fireIntervalRemoved(Object source, int index0, int index1) {
	ListDataEvent e = null;

	for (int i=listeners.size()-1; i>=0; i--) {
		if (e == null) {
		    e = new ListDataEvent(source, ListDataEvent.INTERVAL_REMOVED, index0, index1);
		}
		((ListDataListener)listeners.get(i)).intervalRemoved(e);
	}
}
/**
 * Get process as specified index
 * @param index
 * @return BaciProcess
 */
public BaciProcess get(int index) {
	return (BaciProcess)processes.get(index);
}
/**
  * Gets element at specified index
  * @param index
  * @return Object
  */
public Object getElementAt(int index) {
	return processes.get(index);
}
/** 
  * Gets the number of process in list
  * @return int
  */
public int getSize() {
	return processes.size();
}
/**
 * Remove specified process from list
 * @param index
 */
public void remove(int index) {
	processes.remove(index);
	fireIntervalRemoved(this,index,index);
}
/**
  * Remove a listener from the list that's notified when the list changes
  * @param l listener
  */
public void removeListDataListener(javax.swing.event.ListDataListener l) {
	listeners.remove(l);
}
}
