
#include "common.world.h"
#include "parallel.world.h"
#include "external.world.h"
#include "flatfield.h"






struct StitchRules {

  uint nofIn;
  Crop<2> crp;                  ///< Crop input projection image
  Crop<2> fcrp;                  ///< Crop final projection image
  Binn<2> bnn;                  ///< binning factor
  float angle;                ///< Rotation angle.
  PointF<2> origin1;            ///< Origin of the next image in the first stitch
  PointF<2> origin2;            ///< Origin of the next image in the second stitch
  uint origin1size;
  uint origin2size;           ///< Nof images in the second stitch - needed only if it is requested (origin2)
  PointF<2> originF;            ///< Origin of the flipped portion
  bool flip;               ///< indicates if originF was given in options.
  std::deque<uint> splits;          ///< Split pooints to separate samples.
  uint edge;               ///< blur of mask and image edges.

  StitchRules()
  : nofIn(0)
  , crp()
  , angle(0)
  , origin1size(1)
  , origin2size(1)
  , flip(false)
  , edge(0)
  {}

  void slot(int cur, int* cur1, int* cur2, int* curF) const;

};





class ProcProj {

  static const std::string modname;

  // shared, all const except in the constructor.
  const StitchRules & strl;
  const Shape<2> ish; // input image
  const Shape<2> psh; // processed image
  const Shape<2> ssh; // stitched image
  const std::vector<Shape<2>> oshs; // split images
  std::deque<Map> wghts;
  Map swght;
  std::deque<PointF<2>> origins;
  Map mskF;
  CLmem maskCL;

  // own
  std::deque<FlatFieldProc> ffprocs;
  ImageProc iproc;
  std::deque<Map> allIn;
  Map stitched, final;
  std::deque<Map> res; // will all reference final
  bool doGapsFill;
  CLprogram oclProgram;
  CLkernel gaussCL;
  CLmem iomCL;

  void stitchSome( const ImagePath & interim_name = ImagePath()
                 , const std::vector<bool> & addMe = std::vector<bool>());
  void initCL();

public:

  ProcProj( const StitchRules & st, const Shape<2> & ish
          , const std::deque<Map> & bgas, const std::deque<Map> & dfas
          , const std::deque<Map> & dgas, const std::deque<Map> & msas
          , const Path & saveMasks = Path());

  ProcProj(const ProcProj & other);

  std::deque<Map> & process( std::deque<Map> & allInR, const ImagePath & interim_name = ImagePath());

  const std::vector<Shape<2>> & outputShapes() const {return oshs;}
  static std::vector<Shape<2>> outputShapes(const StitchRules & st, const Shape<2> & ish);

};





class Denoiser {

private:
  const Shape<2> sh;
  const int rad;
  const float thr;
  mutable Map tarr;
  Map swghts;
  Map mask;

public:

  Denoiser(const Shape<2> & _sh, int _rad, float _threshold, const Map & _mask=defaultMap);

  Denoiser(const Denoiser & other)
    : sh(other.sh)
    , tarr(sh)
    , swghts(other.swghts)
    , mask(other.mask)
    , rad(other.rad)
    , thr(other.thr)
  {
    if (!size(sh))
      warn("denoiser", "empty input shape.");
  }

  void proc(Map & iom) const ;

};

