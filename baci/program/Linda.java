package baci.program;
import baci.gui.*;
import java.util.*;
import baci.interpreter.*;
import javax.swing.*;
import javax.swing.text.*;

/*
    BACI implementation of a Linda Tuple Space - V1.0 (Pascal).
    Author: Moti Ben-Ari, 2003.
    Tuples are called notes and the tuple space is called the board.
    See jBACI.pdf for more information.
*/  

public class Linda extends PlainDocument {
	// class is derived from PlainDocument like Console
	
	public static final int formal = -32767;    // Indicator of empty or formal parameter 
	private static final Note empty = new Note(formal, formal, formal);
	    
	private ArrayList board = new ArrayList();   // The board of notes
	private ArrayList waiting = new ArrayList(); // List of waiting processes
	    
	public Linda() {
	    super();
	    clearBoard();
	}

	public void clearBoard() {
		board.clear();
		waiting.clear();
		clear();
	}
	
	void writeboard() {
	    clear();
	    for (int i = 0; i < board.size(); i++) 
		    print(" " + (Note)board.get(i) + "\n");
	}
	
	void post(Note t) {
		board.add(t);
		// Awaken all blocked process and set instruction pointer back to read/remove statement
		while (waiting.size() > 0) {
			BaciProcess wakeProc = (BaciProcess) waiting.remove(0);
			wakeProc.setCurrentPcode(wakeProc.getCurrentPcode()-1);
			wakeProc.setSuspend(null);
			wakeProc.setActive(true);
		}
	}
	
	void postnote(int c, int i1, int i2) {
	    post(new Note(c, i1, i2));
	    writeboard();
	}
	  
	int searchnote(Note t) {
	// Search for a match on the board; formal matches anything
	    int i = 0; 
	    boolean found = false;
	    while (!found && (i < board.size())) {
		    Note n = (Note) board.get(i);
		    if (n.note[0] == formal) found = false;
		    else found =
			(t.note[0] == n.note[0]) &&
		    	((t.note[1] == formal) || (n.note[1] == formal) || (t.note[1] == n.note[1])) &&
			((t.note[2] == formal) || (n.note[2] == formal) || (t.note[2] == n.note[2]));
		    if (!found) i++;
	     }
	  return i;
	}
	
	Note getnote(Note t, boolean remove) {
	    int i = searchnote(t);
	    Note n = null;
	    if (i < board.size()) { 
		    n = (Note) board.get(i);
		    if (remove) board.remove(i); 
	     }
	     return n;
	}
	
	Note readremove(BaciProcess currProc, int c, int i1, int i2, boolean remove) {
	    Note t = new Note(c, i1, i2);
	    Note n = getnote(t, remove);
	    if (n == null) waiting.add(currProc); else writeboard();
	    return n;
	}

	// clear and print are overridden from PlainDocument
	
 	public void clear() {
		try { remove(0, getLength()); } catch (BadLocationException ble) { ble.printStackTrace(); }
	}

	public void print(String s) {
		try { insertString(getLength(),s,null); } catch (BadLocationException ble) { ble.printStackTrace(); }
	}
}
