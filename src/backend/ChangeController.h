#pragma once

class ChangeController {
public:
    virtual ~ChangeController() = default;

    virtual void onChange() = 0;
};
