#ifndef HIGH_SPEED_TRAIN_H
#define HIGH_SPEED_TRAIN_H

#include <iostream>
#include <string>

/* DO NOT EDIT HEADER FILE*/

class HighSpeedTrain
{
    private:
        std::string from;
        std::string to;
        int eta;
    public:
        /**
         * Empty Constructor
         * Initializes attributes according to the default route, namely "Florence -> Rome in 90 mins.". 
         **/
        HighSpeedTrain();

        /*
         * Constructor
         * Fills the attributes according to given values.
         *
         * @param from std::string starting location of the travel
         * @param to std::string destination of the travel
         * @param eta int estimated time of arrival in minutes.
         */
        HighSpeedTrain(std::string from, std::string to, int eta);

        /*
         * Copy Constructor
         * Fills the attributes according to attributes of the given object with one exception.
         * Since it requires an extra time to replace a train, the eta of the train will be delayed 60 mins.
         *
         * @param h HighSpeedTrain train that is going to be replaced in the route.
         * @return this new train with updated eta.
         */
        HighSpeedTrain(const HighSpeedTrain &h);

        /*
         * Stream Extraction Overload
         * Formats the output of a given train as "source -> destination in # mins."
         *
         * @param output std::ostream output stream
         * @param h HighSpeedTrain the train whose route is going to be summarized.
         */
        friend std::ostream &operator<<(std::ostream &output, const HighSpeedTrain &h);

        /*
         * Addition Overload
         * This operator will be used to represent transfers.
         * If the destination of second train does not match with the source of the first one,
         * then it produces the string of "Transfer is not possible!" with NO NEW LINE AT THE END.
         * Otherwise it combines the routes and calculate the total eta and produces the string in
         * the form of "source1 -> destination1 -> destination2 in # mins."
         *
         * @param h1 HighSpeedTrain the first train
         * @param h2 HighSpeedTrain the second train
         * @return one of the strings as explained above.
         */
        friend std::string operator+(const HighSpeedTrain &h1, const HighSpeedTrain &h2);

        /*
         * Comparison Overloads
         * This operator will be used to order the summaries of train.
         * The priority for comparison is here:
         * 1. Compare the source of the trains alphabetically (ex. Florence < Milano)
         * 2. Compare the destination of the trains alphabetically (the same example above)
         * 3. Compare the eta's of the trains, the train with less eta will be smaller.
         * 
         * @param h1, h2 HighSpeedTrain the trains to be compared.
         */
        friend bool operator<(const HighSpeedTrain &h1, const HighSpeedTrain &h2);
        friend bool operator>(const HighSpeedTrain &h1, const HighSpeedTrain &h2);
        
        // getters
        inline std::string getFrom(){ return from; }
        inline std::string getTo(){ return to; }
        inline int getETA(){ return eta; }
};

#endif
