#ifndef ABS_JOB_H
#define ABS_JOB_H

class AbsJob {
	public:
		virtual void exec();
		virtual void after();
};

#endif
