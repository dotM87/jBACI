package baci.program;

/*
    BACI implementation of a Linda Tuple Space - V1.0 (Pascal).
    Author: Moti Ben-Ari, 2003.
*/  

public class Note {
    public int [ ] note;

    public Note (int n0, int n1, int n2) {
            note = new int[3];
            note[0] = n0;
            note[1] = n1;
            note[2] = n2;
        }

    public String toString() {
	    String s;
	    if (note[0] == Linda.formal) s = null; 
	    else {
		    s = (char) note[0] + " "; 
		    if (note[1] != Linda.formal) s = s + note[1] + " ";
		    if (note[2] != Linda.formal) s = s + note[2] + " ";
	    }
	    return s;
    }
}

