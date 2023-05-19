
#include <list>
#include "common.h"
#include "flatfield.h"






struct StitchRules {

  uint nofIn;
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

  StitchRules()
  : nofIn(0)
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
  static cl_program oclProgram;

  // shared, all const except in the constructor.
  const StitchRules & strl;
  const Shape ish; // input image
  const Shape psh; // processed image
  const Shape ssh; // stitched image
  const std::vector<Shape> oshs; // split images
  std::deque<Map> wghts;
  Map swght;
  std::deque<PointF2D> origins;
  Map mskF;
  CLmem maskCL;

  // own
  std::deque<FlatFieldProc> ffprocs;
  ImageProc iproc;
  std::deque<Map> allIn;
  Map stitched, final;
  std::deque<Map> res; // will all reference final
  bool doGapsFill;
  CLkernel gaussCL;
  CLmem iomCL;

  void stitchSome( const ImagePath & interim_name = ImagePath()
                 , const std::vector<bool> & addMe = std::vector<bool>());
  void initCL();

public:

  ProcProj( const StitchRules & _st, const Shape & _ish
          , const std::deque<Map> & bgas, const std::deque<Map> & dfas
          , const std::deque<Map> & dgas, const std::deque<Map> & msas
          , const Path & saveMasks = Path());

  ProcProj(const ProcProj & other);

  std::deque<Map> & process( std::deque<Map> & allInR, const ImagePath & interim_name = ImagePath());

  const std::vector<Shape> & outputShapes() const {return oshs;}

};



/*

class Trans {

public:

  static const std::string modname;

  const Shape ish;
  const float angle;
  const Crop crop;
  const PointF2D binn; // negative binn to flip
  Map mask;

  Map afterRot;
  Map afterRotMask;
  void rotate(const Map & in, Map & out);

  Map afterScale;
  Map afterScaleMask;
  void scale(const Map & in, Map & out);

public:

  Trans(const Shape & _ish,
        float _angle,
        const Crop & _crop,
        const PointF2D & _binn,
        const Map & _mask = Map());

  void process(const Map & in, Map & out) {
    if (!area(ish))
      return;
    if (in.shape() != ish)
      throw_error(modname, "Shape missmatch of process ("+toString(ish)+") and input ("+toString(in.shape())+").");
  }

};
const std::string Trans::modname = "transform";


struct Stitch {

  struct Rule {
    Shape ish;
    Crop crp;
    float angle;
    PointF2D origin;
    PointF2D scale; // negative scale to flip
  };

  std::deque<Rule> rules;
  std::deque<FlatFieldProc> ffprocs;
  std::deque<Map> wghts;

  struct Init {
    Rule rule;
    Map bg;
    Map df;
    Map dg;
    Map ms;
  };

  Stitch(const std::deque<Init> & inits, int nofInput = 0) {

  }

};


*/









class Denoiser {

private:
  const Shape sh;
  const int rad;
  const float thr;
  mutable Map tarr;
  Map swghts;
  Map mask;

public:

  Denoiser(const Shape & _sh, int _rad, float _threshold, const Map & _mask=defaultMap);

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

