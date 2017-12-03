class rf_command {
	public: rf_command(unsigned int, unsigned int);
		unsigned int getSender();
		unsigned int  getCommand();
	private:
		unsigned int sender;
		unsigned int  command;
}

class rf_sender {
	public:			rf_sender	(unsigned int); 
		void sendCommand(rf_command);
		rf_command readCommand();

	private: 
		unsigned int id;
};