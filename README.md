# cBirdpp
A C++ library that provides an API wrapper for the eBird 2.0 API.

*The more I get into this, the more it becomes clear that the 2.0 version of the API is unfinished. Until it matures, I see no point in using the 2.0 API over the 1.1 version of the API. I will consider revisiting this project as version 2.0 of the API matures.*

## Current Progress

- [ ] Requests:
  - [x] data/obs
    - [x] Recent observations in a region
    - [x] Recent notable observations in a region
    - [x] Recent observations of a species in a region
    - [x] Recent nearby observations
    - [x] Recent nearby notable observations
        - Note: The detailed version of this test throws an exception. Waiting on response from the eBird API team to see if this is a bug.
    - [x] Recent nearby obserations of a species
    - [x] Nearest observations of a species
    - [x] Historic observations on a date
        - Note: These requests always timeout for me, even on specific, well formed requests. Waiting on response from the eBird API team to see if this is a bug.
  - [x] product
    - [x] Top 100
    - [x] Checklist feed on a date
    - [x] Recent checklists feed
    - [x] Regional statistics on a date
  - [ ] ref/geo
    - [ ] Adjacent Regions
  - [ ] ref/hotspot
    - [ ] Hotspots in a region
    - [ ] Nearby hotspots
  - [ ] ref/taxonomy
    - [ ] eBird Taxonomy
    - [ ] Taxonomic Forms
    - [ ] Taxonomic Versions
    - [ ] Taxonomic Groups
  - [ ] ref/region
    - [ ] Sub Region List

- [ ] Documentation
  - [ ] Setup and installation
  - [ ] Basic usage
  - [ ] Provided types
  - [ ] Requests
  - [ ] Examples

