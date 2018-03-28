/**
 * @file RAIISingleton.h
 *
 * @brief RAIISingleton is an abstract class that provides a framework for
 *        classes that share a single resource between all class instances.
 *
 * This shared resouce inherits from RAIISingleton::SharedResource.  When 
 * a subclass of RAIISingleton is instantiated, the SharedResource will only
 * be created if no other instance of that class exists.  That shared resource
 * will only be destroyed when the last instance of its RAIISingleton is 
 * destroyed.
 *
 * As this application is multi-threaded, all SharedResource implementations
 * should be sure to disallow concurrent access.  This is relatively simple
 * to do safely, as long as all implementations strictly follow these rules:
 *
 */

class RAIISingleton{
	protected:

		

		RAIISingleton(

};
