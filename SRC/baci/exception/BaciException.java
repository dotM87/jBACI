package baci.exception;

/**
 * Wrapper for Exception. Allow BACI specifics if needed.
 * @author: David Strite
 */
public class BaciException extends Exception {
/**
 * Create a new BaciException
 */
public BaciException() {
	super();
}
/**
 * Creates a new BaciException with specified message
 * @param s
 */
public BaciException(String s) {
	super(s);
}
}
