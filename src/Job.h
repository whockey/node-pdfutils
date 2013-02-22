#ifndef ABS_JOB_H
#define ABS_JOB_H

class Job {
	public:
		virtual void exec() = 0;
		virtual void after() = 0;
};

#endif
