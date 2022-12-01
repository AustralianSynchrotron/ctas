
#include "common.h"
#include "flatfield.h"








struct StitchRules {

  uint nofIn;
  Shape ish;
  Crop crp;                  ///< Crop input projection image
  Crop fcrp;                  ///< Crop final projection image
  Binn bnn;                  ///< binning factor
  float angle;                ///< Rotation angle.
  PointF2D origin1;            ///< Origin of the next image in the first stitch
  PointF2D origin2;            ///< Origin of the next image in the second stitch
  uint origin1size;
  uint origin2size;           ///< Nof images in the second stitch - needed only if it is requested (origin2)
  PointF2D originF;            ///< Origin of the flipped portion
  bool flip;               ///< indicates if originF was given in options.
  std::deque<uint> splits;          ///< Split pooints to separate samples.
  uint edge;               ///< blur of mask and image edges.
  float sigma;             ///< sigma used in gaussian gap closure.

  StitchRules()
  : nofIn(0)
  , angle(0)
  , origin1size(1)
  , origin2size(1)
  , flip(false)
  , edge(0)
  , sigma(0)
  {}

  void slot(int cur, int* cur1, int* cur2, int* curF) const;

  Shape outShape() const;

};





class ProcProj {

  static const std::string modname;
  static cl_program oclProgram;

  // shared, all const except in the constructor.
  const StitchRules & strl;
  Shape psh;
  Shape ssh;
  Map wght;
  Map swght;
  std::deque<PointF2D> origins;
  Map mskF;
  CLmem maskCL;

  // own
  std::deque<FlatFieldProc> ffprocs;
  ImageProc iproc;
  std::deque<Map> allIn;
  Map stitched, final;
  bool doGapsFill;
  CLkernel gaussCL;
  CLmem iomCL;

  void stitchSome( const ImagePath & interim_name = ImagePath()
                 , const std::vector<bool> & addMe = std::vector<bool>());
  void prepareMask(Map & _gaps, bool bepicky);
  void initCL();

public:

  ProcProj( const StitchRules & _st
          , const std::deque<Map> & bgas, const std::deque<Map> & dfas
          , const std::deque<Map> & dgas, const std::deque<Map> & msas
          , const Path & saveMasks = Path());

  ProcProj(const ProcProj & other);

  bool process( std::deque<Map> & allInR, std::deque<Map> & res
              , const ImagePath & interim_name = ImagePath());

  Shape outShape() const {
    return strl.outShape();
  }

};



class Denoiser {

private:
  const Shape sh;
  const int rad;
  const float thr;
  mutable Map tarr;
  Map swghts;
  const Map & mask;

public:

  Denoiser(const Shape & _sh, int _rad, float _threshold, const Map & _mask = Map());

  Denoiser(const Denoiser & other)
    : sh(other.sh)
    , tarr(sh)
    , swghts(other.swghts)
    , mask(other.mask)
    , rad(other.rad)
    , thr(other.thr)
  {
    if (!area(sh))
      warn("denoiser", "empty input shape.");
  }

  void proc(Map & iom) const ;

};

