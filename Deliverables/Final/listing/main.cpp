
int main(int argc, char* argv[]) {
	
	using namespace DEBUG;

	OS::Thread controlThread("Control Thread", 
		controlThread, nullptr, OS::Thread::DONT_CARE, OS::Thread::HIGH);
	OS::Thread simulationThread("Simulation Thread", 
		simulationThread, nullptr, OS::Thread::DONT_CARE, OS::Thread::HIGH);

	simulationThread.run();
	controlThread.run();

	simulationThread.join();
	controlThread.join();

	std::cout << "\nDone. Press ENTER to exit.\n";
	std::cin.get();
	return 0;
}