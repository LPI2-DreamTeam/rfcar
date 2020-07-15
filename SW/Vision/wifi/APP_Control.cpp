#include "APP_Control.hpp"


namespace APP {

    Control::Control() {

    }

    Control::~Control() {

    }

    void Control::mainRoutine() {

    }

    void Control::configRoutine() {

    }

    /**
     * @param position
     * @param config
     * @return bool
     */
    bool Control::configIRSensor(IO::Entity<IO::Types::IR_SENSOR>::Position position, IO::Config* config) {
        return false;
    }

    /**
     * @param position
     * @param config
     * @return bool
     */
    bool Control::configMotor(IO::Entity<IO::Types::MOTOR>::Position position, IO::Config* config) {
        return false;
    }

    /**
     * @return bool
     */
    bool Control::configCOMRemoteVision() {
        return false;
    }

    /**
     * @return bool
     */
    bool Control::configCOMSmartphone() {
        return false;
    }
}
