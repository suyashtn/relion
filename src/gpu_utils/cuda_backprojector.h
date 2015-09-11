#ifndef CUDA_BACKPROJECTOR_H_
#define CUDA_BACKPROJECTOR_H_

#include "src/complex.h"
#include "src/gpu_utils/cuda_settings.h"
#include "src/gpu_utils/cuda_mem_utils.h"
#include <cuda_runtime.h>

class CudaBackprojector
{
	int mdlX, mdlY, mdlZ, mdlXYZ,
	    mdlInitY, mdlInitZ,
	    maxR, maxR2,
	    padding_factor;

	XFLOAT *d_mdlReal, *d_mdlImag, *d_mdlWeight;

	cudaStream_t stream;

public:

	CudaBackprojector():
			mdlX(0), mdlY(0), mdlZ(0), mdlXYZ(0),
			mdlInitY(0), mdlInitZ(0),
			maxR(0), maxR2(0),
			padding_factor(0),
			d_mdlReal(0), d_mdlImag(0), d_mdlWeight(0),
			stream(0)
	{}

	CudaBackprojector(
			int xdim, int ydim, int zdim,
			int inity, int initz,
			int max_r, int padding_factor):
				mdlX(0), mdlY(0), mdlZ(0), mdlXYZ(0),
				mdlInitY(0), mdlInitZ(0),
				maxR(0), maxR2(0),
				padding_factor(0),
				d_mdlReal(0), d_mdlImag(0), d_mdlWeight(0),
				stream(0)
	{
		setMdlDim(xdim, ydim, zdim, inity, initz, max_r, padding_factor);
	}

	void setMdlDim(
			int xdim, int ydim, int zdim,
			int inity, int initz,
			int max_r, int paddingFactor)
	{
		mdlX = xdim;
		mdlY = ydim;
		mdlZ = zdim;
		if (mdlZ < 1) mdlZ = 1;
		mdlXYZ = xdim*ydim*zdim;
		mdlInitY = inity;
		mdlInitZ = initz;
		maxR = max_r;
		maxR2 = max_r*max_r;
		padding_factor = paddingFactor;
	}

	void syncStream()
	{
		HANDLE_ERROR(cudaStreamSynchronize(stream));
	}

	void initMdl(int streamPriority);

	void backproject(
			XFLOAT *d_real,
			XFLOAT *d_imag,
			XFLOAT *d_weight,
			XFLOAT *d_eulers,
			int imgX,
			int imgY,
			unsigned long imageCount);

	void getMdlData(XFLOAT *real, XFLOAT *imag, XFLOAT * weights);
	void getMdlData(Complex *data, double * weights);

	cudaStream_t getStream() { return stream; }

	~CudaBackprojector();

};

#endif
